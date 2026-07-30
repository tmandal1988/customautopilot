#include "parameters/parameter_store.h"

#include <bit>
#include <limits>

#include "fcs_params.h"
#include "fcs_parameter_catalog.h"
#include "parameters/parameter_persistence.h"

namespace {

static_assert(sizeof(float) == sizeof(std::uint32_t));
static_assert(std::numeric_limits<float>::is_iec559,
              "MAVLink REAL32 validation requires IEEE-754 float");
static_assert(std::atomic<std::uint32_t>::is_always_lock_free);
static_assert(
    std::atomic<parameters::ParameterRuntimeState>::is_always_lock_free);

enum class Real32Validation : std::uint8_t {
  Valid = 0U,
  NonFinite,
  OutOfRange,
  InvalidMetadata,
};

consteval bool FcsUpdatePoliciesAreIntentional() noexcept {
  constexpr std::uint16_t kBaseMassIndex = static_cast<std::uint16_t>(
      parameters::generated::FcsParameterId::BaseMassKg);
  for (std::uint16_t index = 0U;
       index < parameters::generated::kFcsParameterCount; ++index) {
    const parameters::ParameterUpdatePolicy expected =
        (index == kBaseMassIndex)
            ? parameters::ParameterUpdatePolicy::RebootRequired
            : parameters::ParameterUpdatePolicy::Always;
    if (parameters::generated::kFcsParameterCatalog[index].update_policy !=
        expected) {
      return false;
    }
  }
  return true;
}

static_assert(
    FcsUpdatePoliciesAreIntentional(),
    "Flight-control gains must remain live-tunable; BASE_MASS_KG requires "
    "a reboot. Update the catalog generator policy when regenerating code.");

std::uint32_t FloatToBits(float value) noexcept {
  return std::bit_cast<std::uint32_t>(value);
}

float BitsToFloat(std::uint32_t value) noexcept {
  return std::bit_cast<float>(value);
}

bool IsFiniteReal32(float value) noexcept {
  // This project uses -Ofast/-ffinite-math-only, under which std::isfinite may
  // be folded to true. Inspecting the IEEE-754 exponent bits remains correct
  // for hostile telemetry input under those compiler assumptions.
  constexpr std::uint32_t kExponentMask = 0x7F800000UL;
  return (FloatToBits(value) & kExponentMask) != kExponentMask;
}

std::uint32_t OrderedReal32Key(float value) noexcept {
  constexpr std::uint32_t kSignMask = 0x80000000UL;
  constexpr std::uint32_t kMagnitudeMask = 0x7FFFFFFFUL;
  std::uint32_t bits = FloatToBits(value);
  if ((bits & kMagnitudeMask) == 0U) {
    bits = 0U;  // Treat -0 and +0 as the same inclusive boundary value.
  }
  return ((bits & kSignMask) != 0U) ? ~bits : (bits | kSignMask);
}

Real32Validation ValidateReal32(
    const parameters::ParameterDescriptor& descriptor,
    float value) noexcept {
  if (!IsFiniteReal32(descriptor.minimum_value) ||
      !IsFiniteReal32(descriptor.maximum_value) ||
      (OrderedReal32Key(descriptor.minimum_value) >
       OrderedReal32Key(descriptor.maximum_value))) {
    return Real32Validation::InvalidMetadata;
  }
  if (!IsFiniteReal32(value)) {
    return Real32Validation::NonFinite;
  }
  const std::uint32_t value_key = OrderedReal32Key(value);
  if ((value_key < OrderedReal32Key(descriptor.minimum_value)) ||
      (value_key > OrderedReal32Key(descriptor.maximum_value))) {
    return Real32Validation::OutOfRange;
  }
  return Real32Validation::Valid;
}

}  // namespace

namespace parameters {

ParameterStore& ParameterStore::Instance() noexcept {
  static ParameterStore instance;
  return instance;
}

void ParameterStore::Initialize() noexcept {
  if (ready_.load(std::memory_order_relaxed)) {
    return;
  }

  // Construct/register the persistence worker before TaskManager::StartAll().
  // Restore itself is synchronous here, before any task can observe the store.
  auto& persistence = ParameterPersistence::Instance();
  AssignFcsCtrlParams(fcs_model_parameters_.fcsParamsArg);

  // Necessary one-time catalog enumeration. The periodic control path never
  // scans the catalog or copies the parameter bus.
  for (std::uint16_t index = 0U; index < kParameterCount;
       ++index) {
    float value = 0.0F;
    const ParameterDescriptor* descriptor = Descriptor(index);
    if (!generated::ReadFcsParameter(
            static_cast<generated::FcsParameterId>(index),
            fcs_model_parameters_.fcsParamsArg, &value) ||
        (descriptor == nullptr) ||
        (ValidateReal32(*descriptor, value) != Real32Validation::Valid)) {
      return;
    }
    const std::uint32_t value_bits = FloatToBits(value);
    default_value_bits_[index] = value_bits;
    published_value_bits_[index].store(value_bits,
                                       std::memory_order_relaxed);
  }

  // Missing/corrupt/incompatible storage deliberately falls back to the
  // validated generated defaults above and never prevents controller startup.
  static_cast<void>(persistence.Restore(*this));

  ready_.store(true, std::memory_order_release);
}

bool ParameterStore::ApplyBootReal32Override(
    const char* id, std::size_t id_length,
    std::uint32_t value_bits) noexcept {
  generated::FcsParameterId parameter_id{};
  if (!generated::FindFcsParameter(id, id_length, &parameter_id)) {
    return false;
  }

  const std::uint16_t index = static_cast<std::uint16_t>(parameter_id);
  const ParameterDescriptor* descriptor = Descriptor(index);
  const float value = BitsToFloat(value_bits);
  if ((descriptor == nullptr) ||
      (descriptor->type != ParameterValueType::Real32) ||
      (descriptor->domain != ParameterDomain::FlightControls) ||
      (descriptor->domain_index != index) ||
      (ValidateReal32(*descriptor, value) != Real32Validation::Valid) ||
      !generated::ApplyFcsParameter(parameter_id, value,
                                    fcs_model_parameters_.fcsParamsArg)) {
    return false;
  }

  published_value_bits_[index].store(value_bits, std::memory_order_relaxed);
  override_mask_[index >> 5U].fetch_or(1UL << (index & 31U),
                                       std::memory_order_relaxed);
  return true;
}

void ParameterStore::ResetBootOverridesToDefaults() noexcept {
  // Recovery-only path used before the scheduler starts when a newer Flash
  // generation fails validation. It never executes in the control loop.
  for (std::uint16_t index = 0U; index < kParameterCount;
       ++index) {
    const float value = BitsToFloat(default_value_bits_[index]);
    static_cast<void>(generated::ApplyFcsParameter(
        static_cast<generated::FcsParameterId>(index), value,
        fcs_model_parameters_.fcsParamsArg));
    published_value_bits_[index].store(default_value_bits_[index],
                                       std::memory_order_relaxed);
  }
  for (auto& word : override_mask_) {
    word.store(0U, std::memory_order_relaxed);
  }
  for (auto& word : dirty_mask_) {
    word.store(0U, std::memory_order_relaxed);
  }
}

bool ParameterStore::IsReady() const noexcept {
  return ready_.load(std::memory_order_acquire);
}

std::uint16_t ParameterStore::Count() const noexcept {
  return kParameterCount;
}

const ParameterDescriptor* ParameterStore::Descriptor(
    std::uint16_t index) const noexcept {
  if (index >= kParameterCount) {
    return nullptr;
  }
  return &generated::kFcsParameterCatalog[index];
}

bool ParameterStore::FindById(const char* id, std::size_t id_length,
                              std::uint16_t* index) const noexcept {
  if (!IsReady() || (index == nullptr)) {
    return false;
  }

  generated::FcsParameterId parameter_id{};
  if (!generated::FindFcsParameter(id, id_length, &parameter_id)) {
    return false;
  }

  *index = static_cast<std::uint16_t>(parameter_id);
  return true;
}

bool ParameterStore::ReadReal32(std::uint16_t index,
                                float* value) const noexcept {
  if (!IsReady() || (value == nullptr) ||
      (index >= kParameterCount)) {
    return false;
  }

  *value = BitsToFloat(
      published_value_bits_[index].load(std::memory_order_acquire));
  return true;
}

ParameterSubmitResult ParameterStore::SubmitReal32Update(
    std::uint16_t catalog_index, float requested_value,
    std::uint32_t token) noexcept {
  if (!IsReady()) {
    return ParameterSubmitResult::NotReady;
  }

  const ParameterDescriptor* descriptor = Descriptor(catalog_index);
  if (descriptor == nullptr) {
    return ParameterSubmitResult::InvalidParameter;
  }
  if (descriptor->type != ParameterValueType::Real32) {
    return ParameterSubmitResult::UnsupportedType;
  }
  if (descriptor->domain != ParameterDomain::FlightControls) {
    return ParameterSubmitResult::WrongDomain;
  }

  switch (ValidateReal32(*descriptor, requested_value)) {
    case Real32Validation::Valid:
      break;
    case Real32Validation::NonFinite:
      return ParameterSubmitResult::NonFinite;
    case Real32Validation::OutOfRange:
      return ParameterSubmitResult::OutOfRange;
    case Real32Validation::InvalidMetadata:
      return ParameterSubmitResult::InvalidParameter;
  }

  switch (descriptor->update_policy) {
    case ParameterUpdatePolicy::Always:
    case ParameterUpdatePolicy::DisarmedOnly:
      break;
    case ParameterUpdatePolicy::RebootRequired:
      break;
    default:
      return ParameterSubmitResult::InvalidParameter;
  }

  // Capping all accepted-but-not-yet-popped requests at the completion-ring
  // capacity guarantees that the real-time consumer can always publish one
  // completion without blocking or pre-checking another queue.
  if (fcs_outstanding_update_count_ >= kFcsUpdateQueueCapacity) {
    const std::uint32_t count =
        fcs_request_overflow_count_.load(std::memory_order_relaxed);
    fcs_request_overflow_count_.store(count + 1U,
                                      std::memory_order_relaxed);
    return ParameterSubmitResult::QueueFull;
  }

  const ParameterUpdateRequest request{
      requested_value, token, catalog_index, descriptor->domain_index};
  if (!fcs_update_requests_.TryPush(request)) {
    const std::uint32_t count =
        fcs_request_overflow_count_.load(std::memory_order_relaxed);
    fcs_request_overflow_count_.store(count + 1U,
                                      std::memory_order_relaxed);
    return ParameterSubmitResult::QueueFull;
  }

  ++fcs_outstanding_update_count_;
  return ParameterSubmitResult::Queued;
}

bool ParameterStore::ApplyOneFcsUpdate(
    ParameterRuntimeState runtime_state) noexcept {
  ObserveRuntimeState(runtime_state);

  ParameterUpdateRequest request;
  if (!fcs_update_requests_.TryPop(request)) {
    return false;
  }

  return ProcessFcsUpdate(request, runtime_state);
}

__attribute__((noinline))
bool ParameterStore::ProcessFcsUpdate(
    const ParameterUpdateRequest& request,
    ParameterRuntimeState runtime_state) noexcept {
  ParameterUpdateCompletion completion{
      0.0F, request.token, request.catalog_index,
      ParameterUpdateStatus::InternalError, 0U};

  const ParameterDescriptor* descriptor = Descriptor(request.catalog_index);
  const bool valid_request =
      (descriptor != nullptr) &&
      (descriptor->domain == ParameterDomain::FlightControls) &&
      (descriptor->type == ParameterValueType::Real32) &&
      (descriptor->domain_index == request.domain_index);

  if (valid_request) {
    const Real32Validation validation =
        ValidateReal32(*descriptor, request.requested_value);
    if (validation == Real32Validation::NonFinite) {
      completion.status = ParameterUpdateStatus::RejectedNonFinite;
    } else if (validation == Real32Validation::OutOfRange) {
      completion.status = ParameterUpdateStatus::RejectedOutOfRange;
    } else if (validation == Real32Validation::Valid) {
      bool policy_allows_update = false;
      bool apply_to_live_model = false;
      switch (descriptor->update_policy) {
        case ParameterUpdatePolicy::Always:
          policy_allows_update = true;
          apply_to_live_model = true;
          break;
        case ParameterUpdatePolicy::DisarmedOnly:
          policy_allows_update =
              (runtime_state == ParameterRuntimeState::Disarmed);
          apply_to_live_model = policy_allows_update;
          if (!policy_allows_update) {
            completion.status =
                ParameterUpdateStatus::RejectedUnsafeState;
          }
          break;
        case ParameterUpdatePolicy::RebootRequired:
          // Keep the configured shadow distinct from the live model bank. The
          // override is applied to the model during the next boot restore.
          policy_allows_update =
              (runtime_state == ParameterRuntimeState::Disarmed);
          if (!policy_allows_update) {
            completion.status =
                ParameterUpdateStatus::RejectedUnsafeState;
          }
          break;
        default:
          break;
      }

      const bool model_update_succeeded =
          !apply_to_live_model ||
          generated::ApplyFcsParameter(
              static_cast<generated::FcsParameterId>(request.domain_index),
              request.requested_value,
              fcs_model_parameters_.fcsParamsArg);
      if (policy_allows_update && model_update_succeeded) {
        completion.applied_value = request.requested_value;
        completion.status = apply_to_live_model
                                ? ParameterUpdateStatus::Applied
                                : ParameterUpdateStatus::StagedForReboot;
        published_value_bits_[request.catalog_index].store(
            FloatToBits(completion.applied_value),
            std::memory_order_release);
        MarkPersistentOverride(request.catalog_index, runtime_state);
      }
    }
  }

  if ((completion.status != ParameterUpdateStatus::Applied) &&
      (completion.status != ParameterUpdateStatus::StagedForReboot) &&
      (request.catalog_index < kParameterCount)) {
    completion.applied_value = BitsToFloat(
        published_value_bits_[request.catalog_index].load(
            std::memory_order_acquire));
  }

  // Structurally guaranteed by the outstanding-update cap above. Never block
  // or overwrite if an invariant violation nevertheless occurs.
  if (!fcs_update_completions_.TryPush(completion)) {
    const std::uint32_t count =
        fcs_completion_overflow_count_.load(std::memory_order_relaxed);
    fcs_completion_overflow_count_.store(count + 1U,
                                         std::memory_order_relaxed);
  }
  return true;
}

void ParameterStore::ObserveRuntimeState(
    ParameterRuntimeState runtime_state) noexcept {
  const ParameterRuntimeState previous =
      runtime_state_.load(std::memory_order_relaxed);
  if (previous == runtime_state) {
    return;
  }
  runtime_state_.store(runtime_state, std::memory_order_release);
  if (runtime_state == ParameterRuntimeState::Disarmed) {
    ParameterPersistence::Instance().NotifyWork();
  }
}

void ParameterStore::MarkPersistentOverride(
    std::uint16_t catalog_index,
    ParameterRuntimeState runtime_state) noexcept {
  const std::size_t word = catalog_index >> 5U;
  const std::uint32_t mask = 1UL << (catalog_index & 31U);
  override_mask_[word].fetch_or(mask, std::memory_order_relaxed);
  dirty_mask_[word].fetch_or(mask, std::memory_order_release);
  // An in-flight update is immediately live but Flash remains deferred. Avoid
  // even waking the idle-priority persistence worker while armed; the first
  // observed transition to Disarmed notifies it in ObserveRuntimeState().
  if (runtime_state == ParameterRuntimeState::Disarmed) {
    ParameterPersistence::Instance().NotifyWork();
  }
}

bool ParameterStore::PopFcsCompletion(
    ParameterUpdateCompletion* completion) noexcept {
  if (completion == nullptr) {
    return false;
  }

  if (!fcs_update_completions_.TryPop(*completion)) {
    return false;
  }

  if (fcs_outstanding_update_count_ > 0U) {
    --fcs_outstanding_update_count_;
  }
  return true;
}

std::uint32_t ParameterStore::FcsRequestOverflowCount() const noexcept {
  return fcs_request_overflow_count_.load(std::memory_order_relaxed);
}

std::uint32_t ParameterStore::FcsCompletionOverflowCount() const noexcept {
  return fcs_completion_overflow_count_.load(std::memory_order_relaxed);
}

fcsModel::InstP_fcsModel_T* ParameterStore::FcsModelParameters() noexcept {
  return &fcs_model_parameters_;
}

}  // namespace parameters
