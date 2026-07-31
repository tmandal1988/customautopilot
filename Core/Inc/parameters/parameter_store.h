#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>

#include "fcsModel.h"
#include "parameter_catalog.h"
#include "parameters/parameter_change_event.h"
#include "parameters/parameter_types.h"
#include "parameters/spsc_ring.h"

namespace parameters {

class ParameterPersistence;

// Central configured-value ownership with deterministic owner activation.
// The protocol task is the sole request producer/completion consumer. Each
// live domain has one generated fixed SPSC pair and its owning task consumes
// at most one request at an explicit execution boundary.
class ParameterStore final {
 public:
  static constexpr std::uint16_t kParameterCount =
      generated::kParameterCount;
  static constexpr bool kHasPersistentParameters =
      generated::kPersistentParameterCount != 0U;
  static constexpr std::size_t kLiveDomainCount =
      generated::kLiveParameterDomainCount;
  static constexpr std::size_t kDomainQueueCapacity = 16U;
  static constexpr std::size_t kChangeEventQueueCapacity = 64U;
  static constexpr std::size_t kPersistenceMaskWordCount =
      (kParameterCount + 31U) / 32U;

  static ParameterStore& Instance() noexcept;

  void Initialize() noexcept;
  bool IsReady() const noexcept;

  std::uint16_t Count() const noexcept;
  const ParameterDescriptor* Descriptor(std::uint16_t index) const noexcept;
  bool FindById(const char* id, std::size_t id_length,
                std::uint16_t* index) const noexcept;

  bool ReadValue(std::uint16_t index, ParameterValue* value) const noexcept;
  bool ReadSnapshotValues(std::uint16_t index,
                          ParameterValue* configured_value,
                          ParameterValue* active_value,
                          std::uint32_t* state_revision) const noexcept;
  bool ReadReal32(std::uint16_t index, float* value) const noexcept;
  bool ReadInt32(generated::ParameterId id, std::int32_t* value) const noexcept;
  bool ReadBool(generated::ParameterId id, bool* value) const noexcept;

  // For OnReboot parameters, `immediate_completion` receives the staged
  // configured value and Completed is returned; it must be non-null. Live
  // values return Queued and complete only after their owner calls
  // CompleteDomainUpdate().
  ParameterSubmitResult SubmitUpdate(
      std::uint16_t catalog_index, ParameterValue requested_value,
      std::uint32_t token,
      ParameterUpdateCompletion* immediate_completion) noexcept;

  // Generic live-domain owner API. These calls are O(1), nonblocking and must
  // each have exactly one owning task per domain. A module performs at most one
  // take/apply/complete sequence at its chosen activation boundary.
  bool TakeOneDomainUpdate(ParameterDomain domain,
                           ParameterUpdateRequest* request) noexcept;
  bool CompleteDomainUpdate(ParameterDomain domain,
                            const ParameterUpdateRequest& request,
                            bool applied) noexcept;

  // FCS convenience wrapper: one queue probe and one generated field write.
  bool ApplyOneFcsUpdate() noexcept;

  // Protocol-task completion and logger handoff. PopOneCompletion performs a
  // bounded round-robin over live domains; neither call runs in a control task.
  bool PopOneCompletion(ParameterUpdateCompletion* completion) noexcept;
  bool RecordChange(const ParameterUpdateCompletion& completion) noexcept;
  bool PopChangeEvent(ParameterChangeEvent* event) noexcept;
  bool MakeConfiguredSnapshotEvent(
      std::uint16_t catalog_index, std::uint64_t timestamp_ms,
      std::uint32_t sequence, std::uint32_t session_epoch,
      std::uint32_t state_revision, ParameterValue configured_value,
      ParameterValue active_value,
      ParameterChangeEvent* event) const noexcept;

  // Logger owns session transitions; MAVLink remains the sole event producer.
  // Owner-side session leases make STOP atomic against a concurrent live
  // application without locking or adding work to an empty owner path.
  std::uint32_t BeginChangeLogSession() noexcept;
  std::uint32_t EndChangeLogSession() noexcept;
  std::uint32_t PendingChangeRecordCount(
      std::uint32_t session_epoch) const noexcept;
  std::uint32_t LatestAssignedChangeSequence() const noexcept;
  std::uint32_t LatestRecordedChangeSequence() const noexcept;
  std::uint32_t LatestFailedChangeSequence() const noexcept;

  void ObservePersistenceFlightState(
      PersistenceFlightState flight_state) noexcept;

  // True when no retained override awaits a Flash commit and no commit is
  // executing (always true when the catalog has no persistent parameters, or
  // when persistence is unavailable and waiting could never succeed). Lets a
  // requested reboot drain pending saves instead of silently dropping them.
  bool PersistenceQuiescent() const noexcept;

  // True only while the persistence worker is executing a Flash append or
  // compaction. Arming is refused during this window because a running
  // sector erase cannot be aborted once takeoff follows.
  bool FlashCommitInProgress() const noexcept;

  std::uint32_t RequestOverflowCount() const noexcept;
  std::uint32_t CompletionOverflowCount() const noexcept;
  std::uint32_t ChangeEventOverflowCount() const noexcept;

  // Integration-only access for binding generated Simulink code to stable
  // model-owned parameter storage; no bus copy occurs in the control loop.
  fcsModel::InstP_fcsModel_T* FcsModelParameters() noexcept;

  ParameterStore(const ParameterStore&) = delete;
  ParameterStore& operator=(const ParameterStore&) = delete;

 private:
  friend class ParameterPersistence;

  ParameterStore() noexcept = default;

  bool ApplyBootOverride(const char* id, std::size_t id_length,
                         ParameterValueType stored_type,
                         std::uint32_t value_bits) noexcept;
  void ResetBootOverridesToDefaults() noexcept;
  void MarkPersistentOverride(std::uint16_t catalog_index) noexcept;
  bool ValidateValue(const ParameterDescriptor& descriptor,
                     ParameterValue value,
                     ParameterSubmitResult* failure) const noexcept;
  bool ResolveLiveDomainSlot(ParameterDomain domain,
                             std::uint16_t* slot) const noexcept;
  bool BuildChangeEvent(const ParameterUpdateCompletion& completion,
                        ParameterChangeEvent* event) const noexcept;
  std::uint32_t AcquireChangeRecordLease() noexcept;
  void ReleaseChangeRecordLease(std::uint32_t session_epoch) noexcept;

  fcsModel::InstP_fcsModel_T fcs_model_parameters_{};
  std::array<std::uint32_t, kParameterCount> default_value_bits_{};
  // Configured values are atomically published to telemetry/persistence. Live
  // values publish after owner application; OnReboot values publish when
  // staged and remain inactive in the module until its next startup.
  std::array<std::atomic<std::uint32_t>, kParameterCount>
      published_value_bits_{};
  std::array<std::atomic<std::uint32_t>, kParameterCount>
      active_value_bits_{};
  // Per-value seqlock and revision. Writers touch these only for an accepted
  // parameter change; Logger uses them only when capturing a session baseline.
  std::array<std::atomic<std::uint32_t>, kParameterCount>
      value_snapshot_guards_{};
  std::array<std::atomic<std::uint32_t>, kParameterCount>
      value_state_revisions_{};
  std::atomic<std::uint32_t> next_state_revision_{0U};
  std::array<std::atomic<std::uint32_t>, kPersistenceMaskWordCount>
      override_mask_{};
  std::array<std::atomic<std::uint32_t>, kPersistenceMaskWordCount>
      dirty_mask_{};

  std::array<SpscRing<ParameterUpdateRequest, kDomainQueueCapacity>,
             kLiveDomainCount>
      update_requests_{};
  std::array<SpscRing<ParameterUpdateCompletion, kDomainQueueCapacity>,
             kLiveDomainCount>
      update_completions_{};
  // Protocol-task-owned admission accounting guarantees completion capacity.
  std::array<std::uint16_t, kLiveDomainCount> outstanding_updates_{};
  std::uint16_t completion_cursor_{0U};

  // The MAVLink task is the single producer; Logger is the single consumer.
  SpscRing<ParameterChangeEvent, kChangeEventQueueCapacity>
      change_events_{};

  std::atomic<std::uint32_t> request_overflow_count_{0U};
  std::atomic<std::uint32_t> completion_overflow_count_{0U};
  std::atomic<std::uint32_t> change_event_overflow_count_{0U};
  // next_change_event_sequence_ is touched only by the MAVLink producer.
  // Zero initialization keeps the complete static store in BSS; RecordChange
  // pre-increments so the first externally visible sequence is one.
  std::uint32_t next_change_event_sequence_{0U};
  std::atomic<std::uint32_t> latest_assigned_change_sequence_{0U};
  std::atomic<std::uint32_t> latest_recorded_change_sequence_{0U};
  std::atomic<std::uint32_t> latest_failed_change_sequence_{0U};
  std::atomic<std::uint32_t> change_log_session_epoch_{0U};
  // Adjacent logging/offline epochs use opposite slots. A lease lives only
  // from owner application through MAVLink event disposition, so STOP waits
  // for the closed epoch while later unrestricted updates use the other slot.
  std::array<std::atomic<std::uint32_t>, 2U>
      pending_change_record_counts_{};
  std::atomic<PersistenceFlightState> persistence_flight_state_{
      PersistenceFlightState::Unknown};
  std::atomic<bool> ready_{false};
};

}  // namespace parameters
