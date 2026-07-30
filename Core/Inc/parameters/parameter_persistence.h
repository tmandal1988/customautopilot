#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>

#include "parameters/parameter_persistence_format.h"
#include "task_manager/task_base.h"

namespace parameters {

class ParameterStore;

// Sole owner of runtime Flash erase/program operations. Boot restore is
// bounded and completes before ParameterStore becomes visible to any task;
// all later work runs below the control and telemetry tasks.
class ParameterPersistence final : public TaskBase {
 public:
  static ParameterPersistence& Instance() noexcept;

  bool Restore(ParameterStore& store) noexcept;
  void NotifyWork() noexcept;
  void Run() override;

  std::uint32_t SuccessfulCommitCount() const noexcept;
  std::uint32_t FlashErrorCount() const noexcept;
  std::uint32_t InvalidBootRecordCount() const noexcept;
  bool IsAvailable() const noexcept;

  ParameterPersistence(const ParameterPersistence&) = delete;
  ParameterPersistence& operator=(const ParameterPersistence&) = delete;

 private:
  struct SectorScanResult {
    bool valid{false};
    bool empty{false};
    bool append_usable{false};
    std::uint32_t generation{0U};
    std::uint32_t snapshot_record_count{0U};
    std::uint32_t snapshot_crc32{0U};
    std::uint32_t next_sequence{0U};
    std::size_t next_record_offset{persistence_format::kFirstRecordOffset};
  };

  ParameterPersistence() noexcept;

  bool BankLayoutSupported() const noexcept;
  bool SupplyIsSafeForProgramming() const noexcept;
  bool CanStartFlashOperation(const ParameterStore& store) const noexcept;
  bool ProbeFlashRegion(std::uint32_t start_address,
                        std::size_t size) noexcept;
  bool InspectSector(std::uint32_t sector_base,
                     SectorScanResult* result) noexcept;
  bool ScanSector(std::uint32_t sector_base, ParameterStore& store,
                  const SectorScanResult& metadata,
                  SectorScanResult* result) noexcept;
  bool ProgramFlashword(std::uint32_t address,
                        const void* aligned_flashword) noexcept;
  bool EraseSector(std::uint32_t sector_base) noexcept;
  bool AppendDirty(ParameterStore& store) noexcept;
  bool CompactTo(std::uint32_t destination_base,
                 ParameterStore& store) noexcept;
  bool HasRoomFor(std::size_t record_count) const noexcept;
  void SelectActiveSector(std::uint32_t sector_base,
                          const SectorScanResult& scan) noexcept;

  std::uint32_t active_sector_base_{0U};
  std::uint32_t initial_sector_base_{0U};
  std::uint32_t active_generation_{0U};
  std::uint32_t next_sequence_{0U};
  std::size_t next_record_offset_{persistence_format::kFirstRecordOffset};
  bool append_usable_{false};
  std::atomic<bool> available_{false};
  std::atomic<std::uint32_t> successful_commit_count_{0U};
  std::atomic<std::uint32_t> flash_error_count_{0U};
  std::atomic<std::uint32_t> invalid_boot_record_count_{0U};
};

}  // namespace parameters
