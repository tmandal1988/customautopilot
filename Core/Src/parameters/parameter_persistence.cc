#include "parameters/parameter_persistence.h"

#include <array>
#include <bit>
#include <cstring>

#include "FreeRTOS.h"
#include "fcsModel_types.h"
#include "parameters/parameter_store.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_flash_ex.h"
#include "task.h"

namespace {

using parameters::persistence_format::ParameterRecord;
using parameters::persistence_format::SectorCommit;
using parameters::persistence_format::SectorHeader;

extern "C" std::uint8_t __parameter_flash_a_start__;
extern "C" std::uint8_t __parameter_flash_a_end__;
extern "C" std::uint8_t __parameter_flash_b_start__;
extern "C" std::uint8_t __parameter_flash_b_end__;

constexpr std::uint32_t kExpectedSectorABase = 0x081C0000UL;
constexpr std::uint32_t kExpectedSectorBBase = 0x081E0000UL;
constexpr std::uint32_t kPersistenceDebounceMs = 2000U;
constexpr std::uint32_t kLowVoltageRecheckMs = 1000U;

constexpr bool PersistenceFlightStateAllowsFlash(
    parameters::PersistenceFlightState state) noexcept {
  return state == parameters::PersistenceFlightState::NotInFlight;
}

static_assert(!PersistenceFlightStateAllowsFlash(
    parameters::PersistenceFlightState::Unknown));
static_assert(PersistenceFlightStateAllowsFlash(
    parameters::PersistenceFlightState::NotInFlight));
static_assert(!PersistenceFlightStateAllowsFlash(
    parameters::PersistenceFlightState::InFlight));

std::uint32_t LinkerAddress(const std::uint8_t* symbol) noexcept {
  return static_cast<std::uint32_t>(
      reinterpret_cast<std::uintptr_t>(symbol));
}

std::uint32_t SectorABase() noexcept {
  return LinkerAddress(&__parameter_flash_a_start__);
}

std::uint32_t SectorAEnd() noexcept {
  return LinkerAddress(&__parameter_flash_a_end__);
}

std::uint32_t SectorBBase() noexcept {
  return LinkerAddress(&__parameter_flash_b_start__);
}

std::uint32_t SectorBEnd() noexcept {
  return LinkerAddress(&__parameter_flash_b_end__);
}

bool IsReservedRange(std::uint32_t address, std::size_t size) noexcept {
  if ((size == 0U) ||
      (size > parameters::persistence_format::kSectorSize * 2U)) {
    return false;
  }
  const std::uint32_t final_address =
      address + static_cast<std::uint32_t>(size - 1U);
  if (final_address < address) {
    return false;
  }
  return ((address >= SectorABase()) && (final_address < SectorAEnd())) ||
         ((address >= SectorBBase()) && (final_address < SectorBEnd()));
}

void InvalidateFlashDataCache(std::uint32_t address,
                              std::size_t size) noexcept {
#if (__DCACHE_PRESENT == 1U)
  if ((SCB->CCR & SCB_CCR_DC_Msk) != 0U) {
    constexpr std::uint32_t kCacheLineSize = 32U;
    const std::uint32_t aligned_address = address & ~(kCacheLineSize - 1U);
    const std::uint32_t end_address =
        (address + static_cast<std::uint32_t>(size) + kCacheLineSize - 1U) &
        ~(kCacheLineSize - 1U);
    SCB_InvalidateDCache_by_Addr(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(aligned_address)),
        static_cast<std::int32_t>(end_address - aligned_address));
  }
#else
  static_cast<void>(address);
  static_cast<void>(size);
#endif
}

template <typename Flashword>
void ReadFlashword(std::uint32_t address, Flashword* destination) noexcept {
  static_assert(sizeof(Flashword) ==
                parameters::persistence_format::kFlashwordSize);
  std::memcpy(destination,
              reinterpret_cast<const void*>(
                  static_cast<std::uintptr_t>(address)),
              sizeof(Flashword));
}

}  // namespace

namespace parameters {

static_assert(generated::kPersistentParameterCount <=
                  persistence_format::kRecordCapacity,
              "The complete override snapshot must fit one parameter sector");
static_assert(static_cast<std::uint8_t>(ParameterValueType::Real32) ==
              persistence_format::kReal32Type);
static_assert(static_cast<std::uint8_t>(ParameterValueType::Int32) ==
              persistence_format::kInt32Type);
static_assert(static_cast<std::uint8_t>(ParameterValueType::Bool) ==
              persistence_format::kBoolType);

ParameterPersistence& ParameterPersistence::Instance() noexcept {
  // Constructed from ParameterStore::Initialize() before TaskManager starts
  // registered tasks, so its task handle exists before any runtime update.
  static ParameterPersistence instance;
  return instance;
}

ParameterPersistence::ParameterPersistence() noexcept
    : TaskBase("ParamPersistence", 1536U, osPriorityIdle) {}

bool ParameterPersistence::BankLayoutSupported() const noexcept {
  const bool linker_layout_matches =
      (SectorABase() == kExpectedSectorABase) &&
      (SectorAEnd() ==
       (kExpectedSectorABase + persistence_format::kSectorSize)) &&
      (SectorBBase() == kExpectedSectorBBase) &&
      (SectorBEnd() ==
       (kExpectedSectorBBase + persistence_format::kSectorSize));
  const bool banks_not_swapped =
      (FLASH->OPTSR_CUR & FLASH_OPTSR_SWAP_BANK_OPT) == 0U;
  return linker_layout_matches && banks_not_swapped;
}

bool ParameterPersistence::SupplyIsSafeForProgramming() const noexcept {
  return __HAL_PWR_GET_FLAG(PWR_FLAG_PVDO) == 0U;
}

bool ParameterPersistence::CanStartFlashOperation(
    const ParameterStore& store) const noexcept {
  return SupplyIsSafeForProgramming() &&
         PersistenceFlightStateAllowsFlash(
             store.persistence_flight_state_.load(std::memory_order_acquire));
}

void ParameterPersistence::RefreshFlightState(
    ParameterStore& store) noexcept {
  // Nonblocking latest-value read in the idle worker. FCS already publishes
  // this topic, so this adds no operation to the control loop and closes the
  // 1 Hz telemetry-observation window before any Flash operation starts.
  if (!fcs_state_subscriber_.copy(latest_fcs_state_)) {
    return;
  }

  PersistenceFlightState observed = PersistenceFlightState::Unknown;
  switch (static_cast<enumStateMachine>(latest_fcs_state_.sm_mode)) {
    case enumStateMachine::INACTIVE:
      observed = PersistenceFlightState::NotInFlight;
      break;
    case enumStateMachine::MTR_ARMED:
    case enumStateMachine::INFLIGHT:
      // Armed on the ground already blocks Flash: takeoff can follow at any
      // moment and a running sector erase cannot be aborted.
      observed = PersistenceFlightState::InFlight;
      break;
    default:
      break;
  }
  store.persistence_flight_state_.store(observed,
                                         std::memory_order_release);
}

bool ParameterPersistence::FlashStillPermitted(
    ParameterStore& store) noexcept {
  RefreshFlightState(store);
  return CanStartFlashOperation(store);
}

bool ParameterPersistence::ProbeFlashRegion(std::uint32_t start_address,
                                            std::size_t size) noexcept {
  if (!IsReservedRange(start_address, size) ||
      ((start_address & (persistence_format::kProbeBlockSize - 1U)) != 0U) ||
      ((size % persistence_format::kProbeBlockSize) != 0U)) {
    return false;
  }

  // The Flash controller's CRC engine reports CRCRDERR without asking the CPU
  // to load a potentially torn ECC word. Probe first; only then read through
  // the memory map. This turns an interrupted Bank-2 write into an invalid
  // sector instead of a boot-time BusFault.
  FLASH_CRCInitTypeDef crc_config{};
  crc_config.TypeCRC = FLASH_CRC_ADDR;
  crc_config.BurstSize = FLASH_CRC_BURST_SIZE_4;
  crc_config.Bank = FLASH_BANK_2;
  crc_config.CRCStartAddr = start_address;
  crc_config.CRCEndAddr =
      start_address + static_cast<std::uint32_t>(size - 1U);

  if (HAL_FLASHEx_Unlock_Bank2() != HAL_OK) {
    return false;
  }
  // Older CubeH7 HALs OR the requested burst value into a nonzero reset field.
  // Clear it explicitly so BURST_SIZE_4 really selects the 128-byte minimum.
  CLEAR_BIT(FLASH->CRCCR2,
            FLASH_CRCCR_CRC_BURST | FLASH_CRCCR_CRC_BY_SECT |
                FLASH_CRCCR_ALL_BANK);
  __HAL_FLASH_CLEAR_FLAG_BANK2(FLASH_FLAG_ALL_ERRORS_BANK2);
  std::uint32_t ignored_crc = 0U;
  const HAL_StatusTypeDef status =
      HAL_FLASHEx_ComputeCRC(&crc_config, &ignored_crc);
  static_cast<void>(HAL_FLASHEx_Lock_Bank2());
  return status == HAL_OK;
}

bool ParameterPersistence::InspectSector(
    std::uint32_t sector_base, SectorScanResult* result) noexcept {
  if (result == nullptr) {
    return false;
  }
  *result = {};

  const std::uint32_t header_address =
      sector_base + persistence_format::kHeaderOffset;
  const std::uint32_t commit_address =
      sector_base + persistence_format::kCommitOffset;
  if (!ProbeFlashRegion(header_address, persistence_format::kProbeBlockSize) ||
      !ProbeFlashRegion(commit_address, persistence_format::kProbeBlockSize)) {
    invalid_boot_record_count_.fetch_add(1U, std::memory_order_relaxed);
    return false;
  }
  InvalidateFlashDataCache(header_address,
                           persistence_format::kProbeBlockSize);
  InvalidateFlashDataCache(commit_address,
                           persistence_format::kProbeBlockSize);

  SectorHeader header{};
  SectorCommit commit{};
  ReadFlashword(header_address, &header);
  ReadFlashword(commit_address, &commit);
  if (persistence_format::IsErased(header) &&
      persistence_format::IsErased(commit)) {
    result->empty = true;
    return true;
  }
  if (!persistence_format::IsValid(header) ||
      !persistence_format::IsValid(commit, header)) {
    invalid_boot_record_count_.fetch_add(1U, std::memory_order_relaxed);
    return true;
  }

  result->valid = true;
  result->generation = header.generation;
  result->snapshot_record_count = commit.snapshot_record_count;
  result->snapshot_crc32 = commit.snapshot_crc32;
  return true;
}

bool ParameterPersistence::ScanSector(
    std::uint32_t sector_base, ParameterStore& store,
    const SectorScanResult& metadata, SectorScanResult* result) noexcept {
  if ((result == nullptr) || !metadata.valid) {
    return false;
  }
  *result = metadata;
  result->append_usable = true;
  result->next_sequence = 0U;
  result->next_record_offset = persistence_format::kFirstRecordOffset;

  std::size_t offset = persistence_format::kFirstRecordOffset;
  std::uint32_t snapshot_crc32 = 0U;
  // The commit authenticates this exact prefix. Any failure inside it rejects
  // the generation so Restore() can fall back to the older committed sector.
  for (std::uint32_t record_index = 0U;
       record_index < metadata.snapshot_record_count;
       ++record_index, offset += persistence_format::kRecordStride) {
    const std::uint32_t record_address =
        sector_base + static_cast<std::uint32_t>(offset);
    if (!ProbeFlashRegion(record_address,
                          persistence_format::kProbeBlockSize)) {
      result->valid = false;
      result->append_usable = false;
      invalid_boot_record_count_.fetch_add(1U, std::memory_order_relaxed);
      return true;
    }
    InvalidateFlashDataCache(record_address,
                             persistence_format::kProbeBlockSize);
    ParameterRecord record{};
    ReadFlashword(record_address, &record);
    if (!persistence_format::IsValid(record)) {
      result->valid = false;
      result->append_usable = false;
      invalid_boot_record_count_.fetch_add(1U, std::memory_order_relaxed);
      return true;
    }

    snapshot_crc32 = persistence_format::Crc32Update(
        snapshot_crc32, &record, offsetof(ParameterRecord, crc32));
    result->next_sequence = record.sequence + 1U;
    if (!store.ApplyBootOverride(
            record.name, sizeof(record.name),
            static_cast<ParameterValueType>(record.value_type),
            record.value_bits)) {
      // Removed/renamed/type-changed/out-of-range entries are ignored while
      // defaults and other valid overrides remain usable.
      invalid_boot_record_count_.fetch_add(1U, std::memory_order_relaxed);
    }
  }
  if (snapshot_crc32 != metadata.snapshot_crc32) {
    result->valid = false;
    result->append_usable = false;
    invalid_boot_record_count_.fetch_add(1U, std::memory_order_relaxed);
    return true;
  }

  // Later records are individually committed append entries. A torn final
  // Flash word makes this sector non-appendable but does not discard its
  // authenticated snapshot or the preceding valid append prefix.
  for (;
       offset < persistence_format::kCommitOffset;
       offset += persistence_format::kRecordStride) {
    const std::uint32_t record_address =
        sector_base + static_cast<std::uint32_t>(offset);
    if (!ProbeFlashRegion(record_address,
                          persistence_format::kProbeBlockSize)) {
      result->append_usable = false;
      result->next_record_offset = persistence_format::kCommitOffset;
      invalid_boot_record_count_.fetch_add(1U, std::memory_order_relaxed);
      return true;
    }
    InvalidateFlashDataCache(record_address,
                             persistence_format::kProbeBlockSize);
    ParameterRecord record{};
    ReadFlashword(record_address, &record);
    if (persistence_format::IsErased(record)) {
      result->next_record_offset = offset;
      return true;
    }
    if (!persistence_format::IsValid(record)) {
      result->append_usable = false;
      result->next_record_offset = persistence_format::kCommitOffset;
      invalid_boot_record_count_.fetch_add(1U, std::memory_order_relaxed);
      return true;
    }

    result->next_sequence = record.sequence + 1U;
    result->next_record_offset = offset + persistence_format::kRecordStride;
    if (!store.ApplyBootOverride(
            record.name, sizeof(record.name),
            static_cast<ParameterValueType>(record.value_type),
            record.value_bits)) {
      // Removed/renamed/type-changed/out-of-range entries are ignored while
      // defaults and other valid overrides remain usable.
      invalid_boot_record_count_.fetch_add(1U, std::memory_order_relaxed);
    }
  }

  result->append_usable = false;
  result->next_record_offset = persistence_format::kCommitOffset;
  return true;
}

void ParameterPersistence::SelectActiveSector(
    std::uint32_t sector_base, const SectorScanResult& scan) noexcept {
  active_sector_base_ = sector_base;
  active_generation_ = scan.generation;
  next_sequence_ = scan.next_sequence;
  next_record_offset_ = scan.next_record_offset;
  append_usable_ = scan.append_usable;
}

bool ParameterPersistence::Restore(ParameterStore& store) noexcept {
  available_.store(false, std::memory_order_relaxed);
  if (!BankLayoutSupported()) {
    return false;
  }

  // Do not begin a future erase/program cycle below 2.85 V. Commit-last and
  // the ECC-safe boot probe still handle an abrupt loss after a write starts.
  PWR_PVDTypeDef pvd{};
  pvd.PVDLevel = PWR_PVDLEVEL_6;
  pvd.Mode = PWR_PVD_MODE_NORMAL;
  HAL_PWR_ConfigPVD(&pvd);
  HAL_PWR_EnablePVD();

  SectorScanResult sector_a{};
  SectorScanResult sector_b{};
  const bool sector_a_readable = InspectSector(SectorABase(), &sector_a);
  const bool sector_b_readable = InspectSector(SectorBBase(), &sector_b);
  if (!sector_a_readable && !sector_b_readable) {
    return false;
  }

  // Prefer an already-erased readable sector if no valid generation survives.
  // This also avoids erasing a sector whose metadata could not be probed.
  if (sector_a_readable && sector_a.empty) {
    initial_sector_base_ = SectorABase();
  } else if (sector_b_readable && sector_b.empty) {
    initial_sector_base_ = SectorBBase();
  } else if (sector_a_readable) {
    initial_sector_base_ = SectorABase();
  } else {
    initial_sector_base_ = SectorBBase();
  }

  const bool a_candidate = sector_a_readable && sector_a.valid;
  const bool b_candidate = sector_b_readable && sector_b.valid;
  std::uint32_t first_base = 0U;
  std::uint32_t second_base = 0U;
  const SectorScanResult* first_metadata = nullptr;
  const SectorScanResult* second_metadata = nullptr;
  if (a_candidate && b_candidate) {
    if (persistence_format::IsSequenceNewer(sector_b.generation,
                                            sector_a.generation)) {
      first_base = SectorBBase();
      first_metadata = &sector_b;
      second_base = SectorABase();
      second_metadata = &sector_a;
    } else {
      first_base = SectorABase();
      first_metadata = &sector_a;
      second_base = SectorBBase();
      second_metadata = &sector_b;
    }
  } else if (a_candidate) {
    first_base = SectorABase();
    first_metadata = &sector_a;
  } else if (b_candidate) {
    first_base = SectorBBase();
    first_metadata = &sector_b;
  }

  const auto try_restore = [this, &store](
                               std::uint32_t sector_base,
                               const SectorScanResult* metadata) noexcept {
    if ((sector_base == 0U) || (metadata == nullptr)) {
      return false;
    }
    SectorScanResult scan{};
    if (ScanSector(sector_base, store, *metadata, &scan) && scan.valid) {
      SelectActiveSector(sector_base, scan);
      return true;
    }
    // ScanSector may already have overlaid records before detecting a corrupt
    // committed snapshot. Restore every native module default provider before
    // trying the older generation.
    store.ResetBootOverridesToDefaults();
    return false;
  };

  bool restored = try_restore(first_base, first_metadata);
  if (!restored) {
    restored = try_restore(second_base, second_metadata);
  }
  available_.store(true, std::memory_order_release);
  return restored;
}

bool ParameterPersistence::ProgramFlashword(
    std::uint32_t address, const void* aligned_flashword) noexcept {
  if ((aligned_flashword == nullptr) ||
      ((address & (persistence_format::kProbeBlockSize - 1U)) != 0U) ||
      ((reinterpret_cast<std::uintptr_t>(aligned_flashword) &
        (persistence_format::kFlashwordSize - 1U)) != 0U) ||
      !IsReservedRange(address, persistence_format::kProbeBlockSize) ||
      !ProbeFlashRegion(address, persistence_format::kProbeBlockSize)) {
    flash_error_count_.fetch_add(1U, std::memory_order_relaxed);
    available_.store(false, std::memory_order_release);
    return false;
  }

  alignas(persistence_format::kFlashwordSize)
      std::array<std::uint8_t, persistence_format::kProbeBlockSize> current{};
  std::memcpy(current.data(),
              reinterpret_cast<const void*>(
                  static_cast<std::uintptr_t>(address)),
              current.size());
  if (!persistence_format::IsErased(current)) {
    flash_error_count_.fetch_add(1U, std::memory_order_relaxed);
    available_.store(false, std::memory_order_release);
    return false;
  }

  if (HAL_FLASHEx_Unlock_Bank2() != HAL_OK) {
    flash_error_count_.fetch_add(1U, std::memory_order_relaxed);
    available_.store(false, std::memory_order_release);
    return false;
  }
  __HAL_FLASH_CLEAR_FLAG_BANK2(FLASH_FLAG_ALL_ERRORS_BANK2);
  const HAL_StatusTypeDef program_status = HAL_FLASH_Program(
      FLASH_TYPEPROGRAM_FLASHWORD, address,
      static_cast<std::uint32_t>(
          reinterpret_cast<std::uintptr_t>(aligned_flashword)));
  static_cast<void>(HAL_FLASHEx_Lock_Bank2());
  InvalidateFlashDataCache(address, persistence_format::kProbeBlockSize);

  const bool verified =
      (program_status == HAL_OK) &&
      ProbeFlashRegion(address, persistence_format::kProbeBlockSize) &&
      (std::memcmp(reinterpret_cast<const void*>(
                       static_cast<std::uintptr_t>(address)),
                   aligned_flashword,
                   persistence_format::kFlashwordSize) == 0);
  if (!verified) {
    flash_error_count_.fetch_add(1U, std::memory_order_relaxed);
    available_.store(false, std::memory_order_release);
  }
  return verified;
}

bool ParameterPersistence::EraseSector(std::uint32_t sector_base) noexcept {
  std::uint32_t sector = 0U;
  if (sector_base == SectorABase()) {
    sector = FLASH_SECTOR_6;
  } else if (sector_base == SectorBBase()) {
    sector = FLASH_SECTOR_7;
  } else {
    return false;
  }

  if (HAL_FLASHEx_Unlock_Bank2() != HAL_OK) {
    flash_error_count_.fetch_add(1U, std::memory_order_relaxed);
    available_.store(false, std::memory_order_release);
    return false;
  }
  __HAL_FLASH_CLEAR_FLAG_BANK2(FLASH_FLAG_ALL_ERRORS_BANK2);
  FLASH_EraseInitTypeDef erase{};
  erase.TypeErase = FLASH_TYPEERASE_SECTORS;
  erase.Banks = FLASH_BANK_2;
  erase.Sector = sector;
  erase.NbSectors = 1U;
  erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  std::uint32_t sector_error = 0xFFFFFFFFUL;
  const HAL_StatusTypeDef erase_status =
      HAL_FLASHEx_Erase(&erase, &sector_error);
  static_cast<void>(HAL_FLASHEx_Lock_Bank2());
  InvalidateFlashDataCache(sector_base, persistence_format::kSectorSize);

  if ((erase_status != HAL_OK) || (sector_error != 0xFFFFFFFFUL) ||
      !ProbeFlashRegion(sector_base, persistence_format::kSectorSize)) {
    flash_error_count_.fetch_add(1U, std::memory_order_relaxed);
    available_.store(false, std::memory_order_release);
    return false;
  }

  // Rare rollover-only erase verification. The controller probe above first
  // guarantees that CPU loads cannot encounter DBECC; this bounded scan then
  // guarantees that no previously programmed word will be reused.
  const auto* words = reinterpret_cast<const std::uint32_t*>(
      static_cast<std::uintptr_t>(sector_base));
  for (std::size_t index = 0U;
       index < (persistence_format::kSectorSize / sizeof(std::uint32_t));
       ++index) {
    if (words[index] != 0xFFFFFFFFUL) {
      flash_error_count_.fetch_add(1U, std::memory_order_relaxed);
      available_.store(false, std::memory_order_release);
      return false;
    }
  }
  return true;
}

bool ParameterPersistence::HasRoomFor(std::size_t record_count) const noexcept {
  if (!append_usable_ ||
      (next_record_offset_ > persistence_format::kCommitOffset)) {
    return false;
  }
  const std::size_t available_bytes =
      persistence_format::kCommitOffset - next_record_offset_;
  return record_count <=
         (available_bytes / persistence_format::kRecordStride);
}

bool ParameterPersistence::CompactTo(std::uint32_t destination_base,
                                     ParameterStore& store) noexcept {
  // The erase is the one unabortable operation; take the freshest possible
  // FCS state immediately before starting it.
  if (!FlashStillPermitted(store)) {
    return false;
  }
  if (!EraseSector(destination_base)) {
    return false;
  }

  const std::uint32_t generation =
      (active_sector_base_ == 0U) ? 1U : (active_generation_ + 1U);
  alignas(persistence_format::kFlashwordSize) const SectorHeader header =
      persistence_format::MakeSectorHeader(generation);
  if (!FlashStillPermitted(store) ||
      !ProgramFlashword(destination_base + persistence_format::kHeaderOffset,
                        &header)) {
    return false;
  }

  std::size_t offset = persistence_format::kFirstRecordOffset;
  std::uint32_t sequence = next_sequence_;
  std::uint32_t snapshot_record_count = 0U;
  std::uint32_t snapshot_crc32 = 0U;
  // Compaction is a rare idle-priority walk over only set override bits. Stable
  // names are persisted rather than catalog indices, so reordering and new
  // domains do not reinterpret old values.
  for (std::size_t word = 0U; word < store.override_mask_.size(); ++word) {
    std::uint32_t remaining =
        store.override_mask_[word].load(std::memory_order_acquire);
    while (remaining != 0U) {
      const std::uint32_t bit = std::countr_zero(remaining);
      remaining &= remaining - 1U;
      const std::size_t index = (word * 32U) + bit;
      if ((index >= store.Count()) ||
          (offset >= persistence_format::kCommitOffset) ||
          !FlashStillPermitted(store)) {
        return false;
      }

      const ParameterDescriptor* descriptor =
          store.Descriptor(static_cast<std::uint16_t>(index));
      if ((descriptor == nullptr) ||
          !persistence_format::IsSupportedValueType(
              static_cast<std::uint8_t>(descriptor->type)) ||
          (descriptor->persistence_policy !=
           ParameterPersistencePolicy::RetainedOverride)) {
        return false;
      }
      const std::uint32_t value_bits =
          store.published_value_bits_[index].load(std::memory_order_acquire);
      alignas(persistence_format::kFlashwordSize) const ParameterRecord record =
          persistence_format::MakeRecord(
              descriptor->name, value_bits,
              static_cast<std::uint8_t>(descriptor->type), sequence);
      if (!ProgramFlashword(
              destination_base + static_cast<std::uint32_t>(offset), &record)) {
        return false;
      }
      ++sequence;
      ++snapshot_record_count;
      snapshot_crc32 = persistence_format::Crc32Update(
          snapshot_crc32, &record, offsetof(ParameterRecord, crc32));
      offset += persistence_format::kRecordStride;
    }
  }

  alignas(persistence_format::kFlashwordSize) const SectorCommit commit =
      persistence_format::MakeSectorCommit(header, snapshot_record_count,
                                            snapshot_crc32);
  if (!FlashStillPermitted(store) ||
      !ProgramFlashword(destination_base + persistence_format::kCommitOffset,
                        &commit)) {
    return false;
  }

  active_sector_base_ = destination_base;
  active_generation_ = generation;
  next_sequence_ = sequence;
  next_record_offset_ = offset;
  append_usable_ = true;
  return true;
}

bool ParameterPersistence::AppendDirty(ParameterStore& store) noexcept {
  if (!FlashStillPermitted(store)) {
    return false;
  }
  using DirtyMask =
      std::array<std::uint32_t, ParameterStore::kPersistenceMaskWordCount>;
  DirtyMask captured{};
  std::size_t dirty_count = 0U;
  for (std::size_t word = 0U; word < captured.size(); ++word) {
    captured[word] =
        store.dirty_mask_[word].exchange(0U, std::memory_order_acq_rel);
    dirty_count += std::popcount(captured[word]);
  }
  if (dirty_count == 0U) {
    return true;
  }

  const auto restore_dirty = [&store, &captured]() noexcept {
    for (std::size_t word = 0U; word < captured.size(); ++word) {
      if (captured[word] != 0U) {
        store.dirty_mask_[word].fetch_or(captured[word],
                                         std::memory_order_release);
      }
    }
  };

  if ((active_sector_base_ == 0U) || !HasRoomFor(dirty_count)) {
    const std::uint32_t destination =
        (active_sector_base_ == SectorABase())
            ? SectorBBase()
            : ((active_sector_base_ == SectorBBase())
                   ? SectorABase()
                   : initial_sector_base_);
    if (destination == 0U) {
      restore_dirty();
      return false;
    }
    if (!CompactTo(destination, store)) {
      restore_dirty();
      return false;
    }
    successful_commit_count_.fetch_add(1U, std::memory_order_relaxed);
    return true;
  }

  // Normal saves append only the final value for each dirty parameter. QGC
  // tuning bursts are therefore coalesced without an erase or a full snapshot.
  for (std::size_t word = 0U; word < captured.size(); ++word) {
    while (captured[word] != 0U) {
      const std::uint32_t bit = std::countr_zero(captured[word]);
      const std::uint32_t mask = 1UL << bit;
      const std::size_t index = (word * 32U) + bit;
      if ((index >= store.Count()) || !FlashStillPermitted(store)) {
        restore_dirty();
        return false;
      }

      const ParameterDescriptor* descriptor =
          store.Descriptor(static_cast<std::uint16_t>(index));
      if ((descriptor == nullptr) ||
          !persistence_format::IsSupportedValueType(
              static_cast<std::uint8_t>(descriptor->type)) ||
          (descriptor->persistence_policy !=
           ParameterPersistencePolicy::RetainedOverride)) {
        restore_dirty();
        return false;
      }
      const std::uint32_t value_bits =
          store.published_value_bits_[index].load(std::memory_order_acquire);
      alignas(persistence_format::kFlashwordSize) const ParameterRecord record =
          persistence_format::MakeRecord(
              descriptor->name, value_bits,
              static_cast<std::uint8_t>(descriptor->type), next_sequence_);
      if (!ProgramFlashword(
              active_sector_base_ +
                  static_cast<std::uint32_t>(next_record_offset_),
              &record)) {
        append_usable_ = false;
        restore_dirty();
        return false;
      }
      ++next_sequence_;
      next_record_offset_ += persistence_format::kRecordStride;
      captured[word] &= ~mask;
    }
  }

  successful_commit_count_.fetch_add(1U, std::memory_order_relaxed);
  return true;
}

void ParameterPersistence::NotifyWork() noexcept {
  if (taskHandle != nullptr) {
    xTaskNotifyGive(static_cast<TaskHandle_t>(taskHandle));
  }
}

void ParameterPersistence::Run() {
  auto& store = ParameterStore::Instance();
  constexpr TickType_t kDebounceTicks =
      pdMS_TO_TICKS(kPersistenceDebounceMs);
  constexpr TickType_t kLowVoltageRecheckTicks =
      pdMS_TO_TICKS(kLowVoltageRecheckMs);
  ConfigureEventMetrics();

  for (;;) {
    BeginMetricsCycle();
    RefreshFlightState(store);
    bool has_dirty = false;
    for (const auto& word : store.dirty_mask_) {
      if (word.load(std::memory_order_acquire) != 0U) {
        has_dirty = true;
        break;
      }
    }

    if (!has_dirty || !available_.load(std::memory_order_acquire) ||
        !PersistenceFlightStateAllowsFlash(
            store.persistence_flight_state_.load(std::memory_order_acquire))) {
      EndMetricsCycle();
      static_cast<void>(ulTaskNotifyTake(pdTRUE, portMAX_DELAY));
      continue;
    }

    EndMetricsCycle();

    // Any update notification restarts the quiet period. With no dirty work
    // this task sleeps indefinitely; there is no periodic polling loop.
    if (ulTaskNotifyTake(pdTRUE, kDebounceTicks) != 0U) {
      continue;
    }
    BeginMetricsCycle();
    RefreshFlightState(store);
    if (!PersistenceFlightStateAllowsFlash(
            store.persistence_flight_state_.load(std::memory_order_acquire))) {
      EndMetricsCycle();
      continue;
    }
    if (!SupplyIsSafeForProgramming()) {
      EndMetricsCycle();
      static_cast<void>(
          ulTaskNotifyTake(pdTRUE, kLowVoltageRecheckTicks));
      continue;
    }
    // The flag is raised before AppendDirty captures the dirty mask, so a
    // quiescence probe that sees an empty mask and a lowered flag can only
    // observe a fully completed commit.
    commit_in_progress_.store(true, std::memory_order_release);
    static_cast<void>(AppendDirty(store));
    commit_in_progress_.store(false, std::memory_order_release);
    EndMetricsCycle();
  }
}

std::uint32_t ParameterPersistence::SuccessfulCommitCount() const noexcept {
  return successful_commit_count_.load(std::memory_order_relaxed);
}

std::uint32_t ParameterPersistence::FlashErrorCount() const noexcept {
  return flash_error_count_.load(std::memory_order_relaxed);
}

std::uint32_t ParameterPersistence::InvalidBootRecordCount() const noexcept {
  return invalid_boot_record_count_.load(std::memory_order_relaxed);
}

bool ParameterPersistence::IsAvailable() const noexcept {
  return available_.load(std::memory_order_acquire);
}

bool ParameterPersistence::IsCommitIdle() const noexcept {
  return !commit_in_progress_.load(std::memory_order_acquire);
}

}  // namespace parameters
