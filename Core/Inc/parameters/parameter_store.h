#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>

#include "parameters/parameter_types.h"
#include "parameters/spsc_ring.h"
#include "fcs_parameter_catalog.h"
#include "fcsModel.h"

namespace parameters {

class ParameterPersistence;

// Owns the authoritative parameter banks. Each mutable bank has exactly one
// domain-task writer; protocol tasks communicate through fixed SPSC rings.
class ParameterStore final {
 public:
  // This is the global catalog size. Extend the generated aggregate when a
  // second owning domain is introduced; persistence itself is domain-agnostic.
  static constexpr std::uint16_t kParameterCount =
      generated::kFcsParameterCount;
  static constexpr std::size_t kFcsUpdateQueueCapacity = 16U;
  static constexpr std::size_t kPersistenceMaskWordCount =
      (kParameterCount + 31U) / 32U;

  static ParameterStore& Instance() noexcept;

  void Initialize() noexcept;
  bool IsReady() const noexcept;

  std::uint16_t Count() const noexcept;
  const ParameterDescriptor* Descriptor(std::uint16_t index) const noexcept;
  bool FindById(const char* id, std::size_t id_length,
                std::uint16_t* index) const noexcept;
  bool ReadReal32(std::uint16_t index, float* value) const noexcept;

  // Task ownership is part of this API contract:
  // - SubmitReal32Update: one protocol-task producer.
  // - ApplyOneFcsUpdate: one FlightControls-task consumer/producer.
  // - PopFcsCompletion: one protocol-task consumer.
  ParameterSubmitResult SubmitReal32Update(std::uint16_t catalog_index,
                                           float requested_value,
                                           std::uint32_t token) noexcept;
  bool ApplyOneFcsUpdate(ParameterRuntimeState runtime_state) noexcept;
  bool PopFcsCompletion(ParameterUpdateCompletion* completion) noexcept;

  std::uint32_t FcsRequestOverflowCount() const noexcept;
  std::uint32_t FcsCompletionOverflowCount() const noexcept;

  // Integration-only access for binding the generated model to stable storage.
  fcsModel::InstP_fcsModel_T* FcsModelParameters() noexcept;

  ParameterStore(const ParameterStore&) = delete;
  ParameterStore& operator=(const ParameterStore&) = delete;

 private:
  friend class ParameterPersistence;

  ParameterStore() noexcept = default;

  bool ApplyBootReal32Override(const char* id, std::size_t id_length,
                               std::uint32_t value_bits) noexcept;
  void ResetBootOverridesToDefaults() noexcept;
  bool ProcessFcsUpdate(const ParameterUpdateRequest& request,
                        ParameterRuntimeState runtime_state) noexcept;
  void ObserveRuntimeState(ParameterRuntimeState runtime_state) noexcept;
  void MarkPersistentOverride(
      std::uint16_t catalog_index,
      ParameterRuntimeState runtime_state) noexcept;

  fcsModel::InstP_fcsModel_T fcs_model_parameters_{};
  // Defaults are immutable after Initialize(); configured values can differ
  // from the active model bank for RebootRequired parameters.
  std::array<std::uint32_t, kParameterCount>
      default_value_bits_{};
  std::array<std::atomic<std::uint32_t>, kParameterCount>
      published_value_bits_{};
  std::array<std::atomic<std::uint32_t>, kPersistenceMaskWordCount>
      override_mask_{};
  std::array<std::atomic<std::uint32_t>, kPersistenceMaskWordCount>
      dirty_mask_{};
  SpscRing<ParameterUpdateRequest, kFcsUpdateQueueCapacity>
      fcs_update_requests_{};
  SpscRing<ParameterUpdateCompletion, kFcsUpdateQueueCapacity>
      fcs_update_completions_{};
  // Submit and completion-pop are both owned by the same protocol task.
  std::uint32_t fcs_outstanding_update_count_{0U};
  std::atomic<std::uint32_t> fcs_request_overflow_count_{0U};
  std::atomic<std::uint32_t> fcs_completion_overflow_count_{0U};
  std::atomic<ParameterRuntimeState> runtime_state_{
      ParameterRuntimeState::Unknown};
  std::atomic<bool> ready_{false};
};

}  // namespace parameters
