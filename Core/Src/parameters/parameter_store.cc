#include "parameters/parameter_store.h"

#include <bit>
#include <cstring>

#include "fcs_parameter_catalog.h"
#include "fcs_params.h"
#include "parameters/parameter_persistence.h"
#include "pubsub/topic.h"

namespace {

static_assert(std::atomic<std::uint32_t>::is_always_lock_free);
static_assert(
    std::atomic<parameters::PersistenceFlightState>::is_always_lock_free);
static_assert(parameters::generated::kFcsPersistentParameterCount == 0U,
              "FCS values must always boot from fcs_params.h");
static_assert(parameters::ParameterStore::kLiveDomainCount > 0U);

consteval bool CatalogPoliciesAreValid() noexcept {
  for (const auto& descriptor :
       parameters::generated::kParameterCatalog) {
    if ((descriptor.activation_policy ==
         parameters::ParameterActivationPolicy::OnReboot) &&
        (descriptor.persistence_policy ==
         parameters::ParameterPersistencePolicy::Volatile)) {
      return false;
    }
  }
  for (const auto& descriptor :
       parameters::generated::kFcsParameterCatalog) {
    if ((descriptor.domain != parameters::ParameterDomain::FlightControls) ||
        (descriptor.type != parameters::ParameterValueType::Real32) ||
        (descriptor.activation_policy !=
         parameters::ParameterActivationPolicy::Live) ||
        (descriptor.persistence_policy !=
         parameters::ParameterPersistencePolicy::Volatile)) {
      return false;
    }
  }
  return true;
}

static_assert(CatalogPoliciesAreValid(),
              "Generated parameter policies violate runtime invariants");

bool IsFiniteReal32Bits(std::uint32_t bits) noexcept {
  constexpr std::uint32_t kExponentMask = 0x7F800000UL;
  return (bits & kExponentMask) != kExponentMask;
}

std::uint32_t OrderedReal32Key(std::uint32_t bits) noexcept {
  constexpr std::uint32_t kSignMask = 0x80000000UL;
  constexpr std::uint32_t kMagnitudeMask = 0x7FFFFFFFUL;
  if ((bits & kMagnitudeMask) == 0U) {
    bits = 0U;  // Treat negative and positive zero identically.
  }
  return ((bits & kSignMask) != 0U) ? ~bits : (bits | kSignMask);
}

parameters::ParameterUpdateStatus ValidationFailureStatus(
    parameters::ParameterSubmitResult failure) noexcept {
  switch (failure) {
    case parameters::ParameterSubmitResult::NonFinite:
      return parameters::ParameterUpdateStatus::RejectedNonFinite;
    case parameters::ParameterSubmitResult::OutOfRange:
      return parameters::ParameterUpdateStatus::RejectedOutOfRange;
    default:
      return parameters::ParameterUpdateStatus::InternalError;
  }
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

  // This is the sole authoritative FCS default provider. Generated protocol
  // defaults are metadata only; the native double-precision model bus is never
  // reconstructed from rounded REAL32 values.
  AssignFcsCtrlParams(fcs_model_parameters_.fcsParamsArg);

  // One bounded startup walk seeds the configured catalog. No catalog walk,
  // parameter-bus copy, allocation, lock or Flash operation exists in a module
  // steady-state path.
  for (std::uint16_t index = 0U; index < kParameterCount; ++index) {
    const ParameterDescriptor* descriptor = Descriptor(index);
    if (descriptor == nullptr) {
      return;
    }

    ParameterValue value{generated::kParameterDefaultBits[index]};
    if (descriptor->domain == ParameterDomain::FlightControls) {
      float fcs_value = 0.0F;
      if ((descriptor->domain_index >= generated::kFcsParameterCount) ||
          !generated::ReadFcsParameter(
              static_cast<generated::FcsParameterId>(
                  descriptor->domain_index),
              fcs_model_parameters_.fcsParamsArg, &fcs_value)) {
        return;
      }
      value = MakeReal32Value(fcs_value);
    }

    ParameterSubmitResult failure = ParameterSubmitResult::InvalidParameter;
    if (!ValidateValue(*descriptor, value, &failure)) {
      return;
    }
    default_value_bits_[index] = value.bits;
    published_value_bits_[index].store(value.bits,
                                       std::memory_order_relaxed);
    active_value_bits_[index].store(value.bits,
                                    std::memory_order_relaxed);
  }

  if constexpr (kHasPersistentParameters) {
    // Restore is synchronous before tasks start. Missing, incompatible or
    // corrupt media deliberately leaves validated module defaults in place.
    auto& persistence = ParameterPersistence::Instance();
    static_cast<void>(persistence.Restore(*this));
  }

  ready_.store(true, std::memory_order_release);
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
  return &generated::kParameterCatalog[index];
}

bool ParameterStore::FindById(const char* id, std::size_t id_length,
                              std::uint16_t* index) const noexcept {
  if (!IsReady() || (index == nullptr)) {
    return false;
  }
  generated::ParameterId parameter_id{};
  if (!generated::FindParameter(id, id_length, &parameter_id)) {
    return false;
  }
  const std::uint16_t resolved = generated::ParameterIndex(parameter_id);
  if (resolved >= kParameterCount) {
    return false;
  }
  *index = resolved;
  return true;
}

bool ParameterStore::ReadValue(std::uint16_t index,
                               ParameterValue* value) const noexcept {
  if (!IsReady() || (value == nullptr) || (index >= kParameterCount)) {
    return false;
  }
  value->bits =
      published_value_bits_[index].load(std::memory_order_acquire);
  return true;
}

bool ParameterStore::ReadReal32(std::uint16_t index,
                                float* value) const noexcept {
  const ParameterDescriptor* descriptor = Descriptor(index);
  ParameterValue raw{};
  if ((value == nullptr) || (descriptor == nullptr) ||
      (descriptor->type != ParameterValueType::Real32) ||
      !ReadValue(index, &raw)) {
    return false;
  }
  *value = AsReal32(raw);
  return true;
}

bool ParameterStore::ReadSnapshotValues(
    std::uint16_t index, ParameterValue* configured_value,
    ParameterValue* active_value,
    std::uint32_t* state_revision) const noexcept {
  if (!IsReady() || (configured_value == nullptr) ||
      (active_value == nullptr) || (state_revision == nullptr) ||
      (index >= kParameterCount)) {
    return false;
  }
  const ParameterDescriptor* descriptor = Descriptor(index);
  if (descriptor == nullptr) {
    return false;
  }

  // Logger-only retry. A writer holds the odd guard only for a few lock-free
  // stores on an accepted parameter change; the FCS empty path never touches
  // this guard and never waits for this reader.
  for (;;) {
    const std::uint32_t guard_before =
        value_snapshot_guards_[index].load(std::memory_order_acquire);
    if ((guard_before & 1U) != 0U) {
      continue;
    }

    configured_value->bits =
        published_value_bits_[index].load(std::memory_order_acquire);
    if (descriptor->activation_policy == ParameterActivationPolicy::Live) {
      // A live value is published only after its owner has applied it.
      *active_value = *configured_value;
    } else {
      active_value->bits =
          active_value_bits_[index].load(std::memory_order_acquire);
    }
    *state_revision =
        value_state_revisions_[index].load(std::memory_order_relaxed);

    const std::uint32_t guard_after =
        value_snapshot_guards_[index].load(std::memory_order_acquire);
    if ((guard_before == guard_after) && ((guard_after & 1U) == 0U)) {
      return true;
    }
  }
}

bool ParameterStore::ReadInt32(generated::ParameterId id,
                               std::int32_t* value) const noexcept {
  const std::uint16_t index = generated::ParameterIndex(id);
  const ParameterDescriptor* descriptor = Descriptor(index);
  ParameterValue raw{};
  if ((value == nullptr) || (descriptor == nullptr) ||
      (descriptor->type != ParameterValueType::Int32) ||
      !ReadValue(index, &raw)) {
    return false;
  }
  *value = AsInt32(raw);
  return true;
}

bool ParameterStore::ReadBool(generated::ParameterId id,
                              bool* value) const noexcept {
  const std::uint16_t index = generated::ParameterIndex(id);
  const ParameterDescriptor* descriptor = Descriptor(index);
  ParameterValue raw{};
  if ((value == nullptr) || (descriptor == nullptr) ||
      (descriptor->type != ParameterValueType::Bool) ||
      !ReadValue(index, &raw) || (raw.bits > 1U)) {
    return false;
  }
  *value = AsBool(raw);
  return true;
}

bool ParameterStore::ValidateValue(
    const ParameterDescriptor& descriptor, ParameterValue value,
    ParameterSubmitResult* failure) const noexcept {
  const auto reject = [failure](ParameterSubmitResult result) noexcept {
    if (failure != nullptr) {
      *failure = result;
    }
    return false;
  };

  switch (descriptor.type) {
    case ParameterValueType::Real32: {
      if (!IsFiniteReal32Bits(descriptor.minimum_bits) ||
          !IsFiniteReal32Bits(descriptor.maximum_bits) ||
          (OrderedReal32Key(descriptor.minimum_bits) >
           OrderedReal32Key(descriptor.maximum_bits))) {
        return reject(ParameterSubmitResult::InvalidParameter);
      }
      if (!IsFiniteReal32Bits(value.bits)) {
        return reject(ParameterSubmitResult::NonFinite);
      }
      const std::uint32_t key = OrderedReal32Key(value.bits);
      if ((key < OrderedReal32Key(descriptor.minimum_bits)) ||
          (key > OrderedReal32Key(descriptor.maximum_bits))) {
        return reject(ParameterSubmitResult::OutOfRange);
      }
      break;
    }
    case ParameterValueType::Int32: {
      const std::int32_t minimum =
          AsInt32(ParameterValue{descriptor.minimum_bits});
      const std::int32_t maximum =
          AsInt32(ParameterValue{descriptor.maximum_bits});
      const std::int32_t requested = AsInt32(value);
      if (minimum > maximum) {
        return reject(ParameterSubmitResult::InvalidParameter);
      }
      if ((requested < minimum) || (requested > maximum)) {
        return reject(ParameterSubmitResult::OutOfRange);
      }
      break;
    }
    case ParameterValueType::Bool:
      if ((descriptor.minimum_bits > descriptor.maximum_bits) ||
          (descriptor.maximum_bits > 1U)) {
        return reject(ParameterSubmitResult::InvalidParameter);
      }
      if ((value.bits > 1U) || (value.bits < descriptor.minimum_bits) ||
          (value.bits > descriptor.maximum_bits)) {
        return reject(ParameterSubmitResult::OutOfRange);
      }
      break;
    default:
      return reject(ParameterSubmitResult::UnsupportedType);
  }

  if (failure != nullptr) {
    *failure = ParameterSubmitResult::Completed;
  }
  return true;
}

bool ParameterStore::ResolveLiveDomainSlot(ParameterDomain domain,
                                           std::uint16_t* slot) const noexcept {
  return generated::LiveDomainSlot(domain, slot) &&
         (slot != nullptr) && (*slot < kLiveDomainCount);
}

std::uint32_t ParameterStore::AcquireChangeRecordLease() noexcept {
  // Lock-free read-side epoch acquisition. If START/STOP advances the epoch
  // between the first sample and counter publication, undo and retry in the
  // new slot. This loop runs only for a real accepted update, never during an
  // empty control cycle, and retries only across a rare session transition.
  for (;;) {
    const std::uint32_t epoch =
        change_log_session_epoch_.load(std::memory_order_acquire);
    auto& count = pending_change_record_counts_[epoch & 1U];
    count.fetch_add(1U, std::memory_order_acq_rel);
    if (change_log_session_epoch_.load(std::memory_order_acquire) == epoch) {
      return epoch;
    }
    count.fetch_sub(1U, std::memory_order_release);
  }
}

void ParameterStore::ReleaseChangeRecordLease(
    std::uint32_t session_epoch) noexcept {
  pending_change_record_counts_[session_epoch & 1U].fetch_sub(
      1U, std::memory_order_release);
}

ParameterSubmitResult ParameterStore::SubmitUpdate(
    std::uint16_t catalog_index, ParameterValue requested_value,
    std::uint32_t token,
    ParameterUpdateCompletion* immediate_completion) noexcept {
  if (!IsReady()) {
    return ParameterSubmitResult::NotReady;
  }

  const ParameterDescriptor* descriptor = Descriptor(catalog_index);
  if (descriptor == nullptr) {
    return ParameterSubmitResult::InvalidParameter;
  }
  ParameterSubmitResult failure = ParameterSubmitResult::InvalidParameter;
  if (!ValidateValue(*descriptor, requested_value, &failure)) {
    return failure;
  }
  if ((descriptor->persistence_policy ==
       ParameterPersistencePolicy::RetainedOverride) &&
      !ParameterPersistence::Instance().IsAvailable()) {
    return ParameterSubmitResult::PersistenceUnavailable;
  }

  if (descriptor->activation_policy ==
      ParameterActivationPolicy::OnReboot) {
    // A volatile OnReboot value could never survive long enough to activate;
    // the generator rejects it and this check protects corrupted metadata.
    if (descriptor->persistence_policy !=
        ParameterPersistencePolicy::RetainedOverride) {
      return ParameterSubmitResult::InvalidParameter;
    }
    if (immediate_completion == nullptr) {
      return ParameterSubmitResult::InvalidParameter;
    }
    const std::uint32_t application_session_epoch =
        AcquireChangeRecordLease();
    const ParameterValue previous{
        published_value_bits_[catalog_index].load(
            std::memory_order_acquire)};
    value_snapshot_guards_[catalog_index].fetch_add(
        1U, std::memory_order_acq_rel);
    published_value_bits_[catalog_index].store(requested_value.bits,
                                               std::memory_order_release);
    const std::uint32_t state_revision =
        next_state_revision_.fetch_add(1U, std::memory_order_relaxed) + 1U;
    value_state_revisions_[catalog_index].store(
        state_revision, std::memory_order_relaxed);
    value_snapshot_guards_[catalog_index].fetch_add(
        1U, std::memory_order_release);
    MarkPersistentOverride(catalog_index);
    *immediate_completion = ParameterUpdateCompletion{
        getCurrentTimeMs(), requested_value, previous, token,
        state_revision, application_session_epoch, catalog_index,
        ParameterUpdateStatus::StagedForReboot, 1U};
    return ParameterSubmitResult::Completed;
  }

  if (descriptor->activation_policy != ParameterActivationPolicy::Live) {
    return ParameterSubmitResult::InvalidParameter;
  }

  std::uint16_t slot = 0U;
  if (!ResolveLiveDomainSlot(descriptor->domain, &slot)) {
    return ParameterSubmitResult::WrongDomain;
  }
  if (outstanding_updates_[slot] >= kDomainQueueCapacity) {
    request_overflow_count_.fetch_add(1U, std::memory_order_relaxed);
    return ParameterSubmitResult::QueueFull;
  }

  const ParameterUpdateRequest request{
      requested_value, token, 0U, catalog_index, descriptor->domain_index};
  if (!update_requests_[slot].TryPush(request)) {
    request_overflow_count_.fetch_add(1U, std::memory_order_relaxed);
    return ParameterSubmitResult::QueueFull;
  }
  ++outstanding_updates_[slot];
  return ParameterSubmitResult::Queued;
}

bool ParameterStore::TakeOneDomainUpdate(
    ParameterDomain domain, ParameterUpdateRequest* request) noexcept {
  if (request == nullptr) {
    return false;
  }
  std::uint16_t slot = 0U;
  if (!ResolveLiveDomainSlot(domain, &slot) ||
      !update_requests_[slot].TryPop(*request)) {
    return false;
  }
  request->application_session_epoch = AcquireChangeRecordLease();
  return true;
}

bool ParameterStore::CompleteDomainUpdate(
    ParameterDomain domain, const ParameterUpdateRequest& request,
    bool applied) noexcept {
  std::uint16_t slot = 0U;
  if (!ResolveLiveDomainSlot(domain, &slot)) {
    ReleaseChangeRecordLease(request.application_session_epoch);
    return false;
  }

  ParameterValue current{};
  if (request.catalog_index < kParameterCount) {
    current.bits = published_value_bits_[request.catalog_index].load(
        std::memory_order_acquire);
  }
  ParameterUpdateCompletion completion{
      getCurrentTimeMs(), current, current, request.token, 0U,
      request.application_session_epoch, request.catalog_index,
      ParameterUpdateStatus::InternalError, 0U};

  const ParameterDescriptor* descriptor = Descriptor(request.catalog_index);
  const bool valid_route =
      (descriptor != nullptr) && (descriptor->domain == domain) &&
      (descriptor->activation_policy == ParameterActivationPolicy::Live) &&
      (descriptor->domain_index == request.domain_index);
  ParameterSubmitResult failure = ParameterSubmitResult::InvalidParameter;
  if (valid_route && !ValidateValue(*descriptor, request.requested_value,
                                    &failure)) {
    completion.status = ValidationFailureStatus(failure);
  } else if (valid_route && applied) {
    completion.previous_value = current;
    completion.value = request.requested_value;
    completion.status = ParameterUpdateStatus::Applied;
    value_snapshot_guards_[request.catalog_index].fetch_add(
        1U, std::memory_order_acq_rel);
    published_value_bits_[request.catalog_index].store(
        request.requested_value.bits, std::memory_order_release);
    active_value_bits_[request.catalog_index].store(
        request.requested_value.bits, std::memory_order_release);
    completion.state_revision =
        next_state_revision_.fetch_add(1U, std::memory_order_relaxed) + 1U;
    value_state_revisions_[request.catalog_index].store(
        completion.state_revision, std::memory_order_relaxed);
    value_snapshot_guards_[request.catalog_index].fetch_add(
        1U, std::memory_order_release);
    MarkPersistentOverride(request.catalog_index);
    completion.change_record_lease = 1U;
  }

  if (completion.change_record_lease == 0U) {
    // Rejected/non-applied requests have no state change to serialize.
    ReleaseChangeRecordLease(request.application_session_epoch);
  }

  // Completion capacity is structurally guaranteed by protocol-task
  // outstanding admission. Never block or overwrite on invariant failure.
  if (!update_completions_[slot].TryPush(completion)) {
    if (completion.change_record_lease != 0U) {
      ReleaseChangeRecordLease(completion.application_session_epoch);
    }
    completion_overflow_count_.fetch_add(1U, std::memory_order_relaxed);
    return false;
  }
  return true;
}

bool ParameterStore::ApplyOneFcsUpdate() noexcept {
  ParameterUpdateRequest request;
  if (!TakeOneDomainUpdate(ParameterDomain::FlightControls, &request)) {
    return false;
  }

  const ParameterDescriptor* descriptor = Descriptor(request.catalog_index);
  ParameterSubmitResult failure = ParameterSubmitResult::InvalidParameter;
  const bool valid =
      (descriptor != nullptr) &&
      (descriptor->domain == ParameterDomain::FlightControls) &&
      (descriptor->type == ParameterValueType::Real32) &&
      (descriptor->activation_policy == ParameterActivationPolicy::Live) &&
      (descriptor->domain_index == request.domain_index) &&
      (request.domain_index < generated::kFcsParameterCount) &&
      ValidateValue(*descriptor, request.requested_value, &failure);
  const bool applied =
      valid && generated::ApplyFcsParameter(
                   static_cast<generated::FcsParameterId>(
                       request.domain_index),
                   AsReal32(request.requested_value),
                   fcs_model_parameters_.fcsParamsArg);
  return CompleteDomainUpdate(ParameterDomain::FlightControls, request,
                              applied);
}

bool ParameterStore::PopOneCompletion(
    ParameterUpdateCompletion* completion) noexcept {
  if (completion == nullptr) {
    return false;
  }

  std::uint16_t slot = completion_cursor_;
  for (std::size_t checked = 0U; checked < kLiveDomainCount; ++checked) {
    if (update_completions_[slot].TryPop(*completion)) {
      // This function is consumed only by the MAVLink task, which is also the
      // sole ParameterChangeEvent producer. Establish the log disposition in
      // the same order in which protocol completions become visible.
      static_cast<void>(RecordChange(*completion));
      if (outstanding_updates_[slot] > 0U) {
        --outstanding_updates_[slot];
      }
      ++slot;
      if (slot >= kLiveDomainCount) {
        slot = 0U;
      }
      completion_cursor_ = slot;
      return true;
    }
    ++slot;
    if (slot >= kLiveDomainCount) {
      slot = 0U;
    }
  }
  return false;
}

bool ParameterStore::BuildChangeEvent(
    const ParameterUpdateCompletion& completion,
    ParameterChangeEvent* event) const noexcept {
  if (event == nullptr) {
    return false;
  }
  const ParameterDescriptor* descriptor =
      Descriptor(completion.catalog_index);
  if (descriptor == nullptr) {
    return false;
  }

  ParameterChangeKind kind{};
  if (completion.status == ParameterUpdateStatus::Applied) {
    kind = ParameterChangeKind::AppliedLive;
  } else if (completion.status ==
             ParameterUpdateStatus::StagedForReboot) {
    kind = ParameterChangeKind::StagedForReboot;
  } else {
    return false;
  }

  *event = {};
  std::memcpy(event->name, descriptor->name, sizeof(event->name));
  event->timestamp_ms = completion.timestamp_ms;
  event->previous_value_bits = completion.previous_value.bits;
  event->value_bits = completion.value.bits;
  event->active_value_bits =
      (completion.status == ParameterUpdateStatus::Applied)
          ? completion.value.bits
          : active_value_bits_[completion.catalog_index].load(
                std::memory_order_acquire);
  event->session_epoch = completion.application_session_epoch;
  event->state_revision = completion.state_revision;
  event->catalog_index = completion.catalog_index;
  event->type = descriptor->type;
  event->kind = kind;
  return true;
}

bool ParameterStore::RecordChange(
    const ParameterUpdateCompletion& completion) noexcept {
  ParameterChangeEvent event{};
  if (!BuildChangeEvent(completion, &event)) {
    if (completion.change_record_lease != 0U) {
      ReleaseChangeRecordLease(completion.application_session_epoch);
    }
    return false;
  }

  // MAVLink is the unique producer, so assignment and enqueue order are
  // identical without a lock or atomic fetch-add. Publish the assignment
  // after owner application. The completion's session lease already made
  // that application atomic against Logger START/STOP.
  event.sequence = ++next_change_event_sequence_;
  latest_assigned_change_sequence_.store(event.sequence,
                                          std::memory_order_release);
  const bool queued = change_events_.TryPush(event);
  if (!queued) {
    change_event_overflow_count_.fetch_add(1U,
                                           std::memory_order_relaxed);
    latest_failed_change_sequence_.store(event.sequence,
                                          std::memory_order_release);
  }
  // On success the SPSC head publication happens-before this release store;
  // on failure the failed-sequence publication does. STOP uses the matching
  // acquire load to wait until every sequence in its cutoff has a disposition.
  latest_recorded_change_sequence_.store(event.sequence,
                                          std::memory_order_release);
  if (completion.change_record_lease != 0U) {
    ReleaseChangeRecordLease(completion.application_session_epoch);
  }
  return queued;
}

bool ParameterStore::PopChangeEvent(ParameterChangeEvent* event) noexcept {
  return (event != nullptr) && change_events_.TryPop(*event);
}

bool ParameterStore::MakeConfiguredSnapshotEvent(
    std::uint16_t catalog_index, std::uint64_t timestamp_ms,
    std::uint32_t sequence, std::uint32_t session_epoch,
    std::uint32_t state_revision, ParameterValue configured_value,
    ParameterValue active_value,
    ParameterChangeEvent* event) const noexcept {
  if (event == nullptr) {
    return false;
  }
  const ParameterDescriptor* descriptor = Descriptor(catalog_index);
  if (descriptor == nullptr) {
    return false;
  }
  *event = {};
  std::memcpy(event->name, descriptor->name, sizeof(event->name));
  event->timestamp_ms = timestamp_ms;
  event->previous_value_bits = configured_value.bits;
  event->value_bits = configured_value.bits;
  event->active_value_bits = active_value.bits;
  event->sequence = sequence;
  event->session_epoch = session_epoch;
  event->state_revision = state_revision;
  event->catalog_index = catalog_index;
  event->type = descriptor->type;
  event->kind = ParameterChangeKind::ConfiguredSnapshot;
  return true;
}

std::uint32_t ParameterStore::BeginChangeLogSession() noexcept {
  return change_log_session_epoch_.fetch_add(1U,
                                              std::memory_order_acq_rel) +
         1U;
}

std::uint32_t ParameterStore::EndChangeLogSession() noexcept {
  // Return the epoch just closed. Owner-side acquisition either publishes a
  // lease in that epoch before this exchange or retries in the new epoch.
  return change_log_session_epoch_.fetch_add(1U,
                                              std::memory_order_acq_rel);
}

std::uint32_t ParameterStore::PendingChangeRecordCount(
    std::uint32_t session_epoch) const noexcept {
  return pending_change_record_counts_[session_epoch & 1U].load(
      std::memory_order_acquire);
}

std::uint32_t ParameterStore::LatestAssignedChangeSequence() const noexcept {
  return latest_assigned_change_sequence_.load(std::memory_order_acquire);
}

std::uint32_t ParameterStore::LatestRecordedChangeSequence() const noexcept {
  return latest_recorded_change_sequence_.load(std::memory_order_acquire);
}

std::uint32_t ParameterStore::LatestFailedChangeSequence() const noexcept {
  return latest_failed_change_sequence_.load(std::memory_order_acquire);
}

bool ParameterStore::ApplyBootOverride(
    const char* id, std::size_t id_length,
    ParameterValueType stored_type, std::uint32_t value_bits) noexcept {
  generated::ParameterId parameter_id{};
  if (!generated::FindParameter(id, id_length, &parameter_id)) {
    return false;
  }
  const std::uint16_t index = generated::ParameterIndex(parameter_id);
  const ParameterDescriptor* descriptor = Descriptor(index);
  if (descriptor == nullptr) {
    return false;
  }

  // Recognize but intentionally ignore stale records for parameters that have
  // become volatile. In particular, no historical FCS override can ever touch
  // the native model bus.
  if (descriptor->persistence_policy ==
      ParameterPersistencePolicy::Volatile) {
    return true;
  }
  const ParameterValue value{value_bits};
  ParameterSubmitResult failure = ParameterSubmitResult::InvalidParameter;
  if ((descriptor->persistence_policy !=
       ParameterPersistencePolicy::RetainedOverride) ||
      (descriptor->type != stored_type) ||
      !ValidateValue(*descriptor, value, &failure)) {
    return false;
  }

  published_value_bits_[index].store(value.bits,
                                     std::memory_order_relaxed);
  active_value_bits_[index].store(value.bits,
                                  std::memory_order_relaxed);
  override_mask_[index >> 5U].fetch_or(1UL << (index & 31U),
                                       std::memory_order_relaxed);
  return true;
}

void ParameterStore::ResetBootOverridesToDefaults() noexcept {
  // Recovery-only, pre-scheduler path. Re-run native providers so the FCS
  // double bus exactly matches fcs_params.h rather than a REAL32 round trip.
  AssignFcsCtrlParams(fcs_model_parameters_.fcsParamsArg);
  for (std::uint16_t index = 0U; index < kParameterCount; ++index) {
    const ParameterDescriptor* descriptor = Descriptor(index);
    std::uint32_t bits = default_value_bits_[index];
    if ((descriptor != nullptr) &&
        (descriptor->domain == ParameterDomain::FlightControls)) {
      float fcs_value = 0.0F;
      if (generated::ReadFcsParameter(
              static_cast<generated::FcsParameterId>(
                  descriptor->domain_index),
              fcs_model_parameters_.fcsParamsArg, &fcs_value)) {
        bits = MakeReal32Value(fcs_value).bits;
      }
    }
    published_value_bits_[index].store(bits,
                                       std::memory_order_relaxed);
    active_value_bits_[index].store(bits,
                                    std::memory_order_relaxed);
  }
  for (auto& word : override_mask_) {
    word.store(0U, std::memory_order_relaxed);
  }
  for (auto& word : dirty_mask_) {
    word.store(0U, std::memory_order_relaxed);
  }
}

void ParameterStore::MarkPersistentOverride(
    std::uint16_t catalog_index) noexcept {
  if constexpr (!kHasPersistentParameters) {
    static_cast<void>(catalog_index);
    return;
  }
  const ParameterDescriptor* descriptor = Descriptor(catalog_index);
  if ((descriptor == nullptr) ||
      (descriptor->persistence_policy !=
       ParameterPersistencePolicy::RetainedOverride)) {
    return;
  }
  const std::size_t word = catalog_index >> 5U;
  const std::uint32_t mask = 1UL << (catalog_index & 31U);
  override_mask_[word].fetch_or(mask, std::memory_order_relaxed);
  dirty_mask_[word].fetch_or(mask, std::memory_order_release);
  // Application/staging never waits for Flash. Writes are merely notified
  // outside INFLIGHT and execute later in the idle-priority worker.
  if (persistence_flight_state_.load(std::memory_order_acquire) ==
      PersistenceFlightState::NotInFlight) {
    ParameterPersistence::Instance().NotifyWork();
  }
}

void ParameterStore::ObservePersistenceFlightState(
    PersistenceFlightState flight_state) noexcept {
  if constexpr (!kHasPersistentParameters) {
    static_cast<void>(flight_state);
    return;
  }
  const PersistenceFlightState previous =
      persistence_flight_state_.load(std::memory_order_relaxed);
  if (previous == flight_state) {
    return;
  }
  persistence_flight_state_.store(flight_state, std::memory_order_release);
  if (flight_state == PersistenceFlightState::NotInFlight) {
    ParameterPersistence::Instance().NotifyWork();
  }
}

bool ParameterStore::PersistenceQuiescent() const noexcept {
  if constexpr (!kHasPersistentParameters) {
    return true;
  }

  // Order matters: read the dirty mask before the commit flag. The worker
  // raises the flag before capturing dirty bits and lowers it only after the
  // Flash write, so "mask empty, flag lowered" proves the commit finished.
  for (const auto& word : dirty_mask_) {
    if (word.load(std::memory_order_acquire) != 0U) {
      // Unsaved work blocks quiescence only while a commit can still happen.
      return !ParameterPersistence::Instance().IsAvailable();
    }
  }
  return ParameterPersistence::Instance().IsCommitIdle();
}

bool ParameterStore::FlashCommitInProgress() const noexcept {
  if constexpr (!kHasPersistentParameters) {
    return false;
  }
  return !ParameterPersistence::Instance().IsCommitIdle();
}

std::uint32_t ParameterStore::RequestOverflowCount() const noexcept {
  return request_overflow_count_.load(std::memory_order_relaxed);
}

std::uint32_t ParameterStore::CompletionOverflowCount() const noexcept {
  return completion_overflow_count_.load(std::memory_order_relaxed);
}

std::uint32_t ParameterStore::ChangeEventOverflowCount() const noexcept {
  return change_event_overflow_count_.load(std::memory_order_relaxed);
}

fcsModel::InstP_fcsModel_T* ParameterStore::FcsModelParameters() noexcept {
  return &fcs_model_parameters_;
}

}  // namespace parameters
