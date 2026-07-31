#pragma once

#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace parameters::persistence_format {

// STM32H743 Flash is programmed one 256-bit Flash word at a time. Keeping
// every on-media object exactly one Flash word makes incomplete writes easy to
// reject and prevents an entry from sharing an ECC word with another entry.
inline constexpr std::size_t kFlashwordSize = 32U;
// The STM32H743 Flash CRC engine reads at least four 256-bit Flash words and
// rounds an address range out to that burst boundary. Give every on-media
// object its own probe block so an ECC-torn write cannot poison a neighbor.
inline constexpr std::size_t kProbeBlockSize = 4U * kFlashwordSize;
inline constexpr std::size_t kRecordStride = kProbeBlockSize;
inline constexpr std::size_t kSectorSize = 128U * 1024U;
inline constexpr std::size_t kParameterNameSize = 16U;
inline constexpr std::size_t kHeaderOffset = 0U;
inline constexpr std::size_t kFirstRecordOffset = kProbeBlockSize;
inline constexpr std::size_t kCommitOffset = kSectorSize - kProbeBlockSize;
inline constexpr std::size_t kRecordCapacity =
    (kCommitOffset - kFirstRecordOffset) / kRecordStride;

inline constexpr std::uint32_t kSectorMagic = 0x31524D50UL;  // "PMR1"
inline constexpr std::uint32_t kCommitMagic = 0x54494D43UL;  // "CMIT"
inline constexpr std::uint16_t kRecordMagic = 0xA56CU;
inline constexpr std::uint16_t kFormatVersion = 2U;
inline constexpr std::uint8_t kReal32Type = 0U;
inline constexpr std::uint8_t kInt32Type = 1U;
inline constexpr std::uint8_t kBoolType = 2U;
inline constexpr std::uint8_t kSetOperation = 1U;

struct alignas(kFlashwordSize) SectorHeader {
  std::uint32_t magic;
  std::uint16_t format_version;
  std::uint16_t header_size;
  std::uint32_t generation;
  std::uint32_t generation_complement;
  std::uint32_t record_stride;
  std::uint32_t sector_size;
  std::uint32_t crc32;
  std::uint32_t crc32_complement;
};

struct alignas(kFlashwordSize) ParameterRecord {
  char name[kParameterNameSize];
  std::uint32_t value_bits;
  std::uint32_t sequence;
  std::uint16_t magic;
  std::uint8_t value_type;
  std::uint8_t operation;
  std::uint32_t crc32;
};

struct alignas(kFlashwordSize) SectorCommit {
  std::uint32_t magic;
  std::uint32_t generation;
  std::uint32_t header_crc32;
  std::uint32_t snapshot_record_count;
  std::uint32_t snapshot_crc32;
  std::uint32_t snapshot_crc32_complement;
  std::uint32_t crc32;
  std::uint32_t crc32_complement;
};

static_assert(std::endian::native == std::endian::little,
              "Parameter media format requires a little-endian target");
static_assert(sizeof(SectorHeader) == kFlashwordSize);
static_assert(sizeof(ParameterRecord) == kFlashwordSize);
static_assert(sizeof(SectorCommit) == kFlashwordSize);
static_assert((kSectorSize % kProbeBlockSize) == 0U);
static_assert((kHeaderOffset % kProbeBlockSize) == 0U);
static_assert((kFirstRecordOffset % kProbeBlockSize) == 0U);
static_assert((kCommitOffset % kProbeBlockSize) == 0U);
static_assert(alignof(SectorHeader) == kFlashwordSize);
static_assert(alignof(ParameterRecord) == kFlashwordSize);
static_assert(alignof(SectorCommit) == kFlashwordSize);
static_assert(std::is_trivially_copyable_v<SectorHeader>);
static_assert(std::is_trivially_copyable_v<ParameterRecord>);
static_assert(std::is_trivially_copyable_v<SectorCommit>);

// Standard reflected CRC-32 (Ethernet/ZIP), used only during boot and by the
// idle-priority persistence task. It is intentionally independent of the MCU
// CRC peripheral so the format is host-testable and portable across boards.
// The update form permits a CRC to cover several non-contiguous buffers
// without allocating or copying them. Pass zero for the first buffer.
inline std::uint32_t Crc32Update(std::uint32_t previous_crc,
                                 const void* data,
                                 std::size_t size) noexcept {
  const auto* bytes = static_cast<const std::uint8_t*>(data);
  std::uint32_t crc = ~previous_crc;
  for (std::size_t index = 0U; index < size; ++index) {
    crc ^= bytes[index];
    for (std::uint8_t bit = 0U; bit < 8U; ++bit) {
      const std::uint32_t reflected_polynomial =
          ((crc & 1U) != 0U) ? 0xEDB88320UL : 0U;
      crc = (crc >> 1U) ^ reflected_polynomial;
    }
  }
  return ~crc;
}

inline std::uint32_t Crc32(const void* data, std::size_t size) noexcept {
  return Crc32Update(0U, data, size);
}

template <typename Flashword>
inline bool IsErased(const Flashword& value) noexcept {
  static_assert(sizeof(Flashword) > 0U);
  const auto* bytes = reinterpret_cast<const std::uint8_t*>(&value);
  for (std::size_t index = 0U; index < sizeof(Flashword); ++index) {
    if (bytes[index] != 0xFFU) {
      return false;
    }
  }
  return true;
}

inline SectorHeader MakeSectorHeader(std::uint32_t generation) noexcept {
  SectorHeader header{};
  header.magic = kSectorMagic;
  header.format_version = kFormatVersion;
  header.header_size = sizeof(SectorHeader);
  header.generation = generation;
  header.generation_complement = ~generation;
  header.record_stride = kRecordStride;
  header.sector_size = kSectorSize;
  header.crc32 = Crc32(&header, offsetof(SectorHeader, crc32));
  header.crc32_complement = ~header.crc32;
  return header;
}

inline bool IsValid(const SectorHeader& header) noexcept {
  return (header.magic == kSectorMagic) &&
         (header.format_version == kFormatVersion) &&
         (header.header_size == sizeof(SectorHeader)) &&
         (header.generation_complement == ~header.generation) &&
         (header.record_stride == kRecordStride) &&
         (header.sector_size == kSectorSize) &&
         (header.crc32_complement == ~header.crc32) &&
         (header.crc32 ==
          Crc32(&header, offsetof(SectorHeader, crc32)));
}

inline SectorCommit MakeSectorCommit(const SectorHeader& header,
                                     std::uint32_t snapshot_record_count,
                                     std::uint32_t snapshot_crc32) noexcept {
  SectorCommit commit{};
  commit.magic = kCommitMagic;
  commit.generation = header.generation;
  commit.header_crc32 = header.crc32;
  commit.snapshot_record_count = snapshot_record_count;
  commit.snapshot_crc32 = snapshot_crc32;
  commit.snapshot_crc32_complement = ~snapshot_crc32;
  commit.crc32 = Crc32(&commit, offsetof(SectorCommit, crc32));
  commit.crc32_complement = ~commit.crc32;
  return commit;
}

inline bool IsValid(const SectorCommit& commit,
                    const SectorHeader& header) noexcept {
  return (commit.magic == kCommitMagic) &&
         (commit.generation == header.generation) &&
         (commit.header_crc32 == header.crc32) &&
         (commit.snapshot_record_count <= kRecordCapacity) &&
         (commit.snapshot_crc32_complement == ~commit.snapshot_crc32) &&
         (commit.crc32_complement == ~commit.crc32) &&
         (commit.crc32 ==
          Crc32(&commit, offsetof(SectorCommit, crc32)));
}

inline bool IsSupportedValueType(std::uint8_t value_type) noexcept {
  return (value_type == kReal32Type) || (value_type == kInt32Type) ||
         (value_type == kBoolType);
}

inline ParameterRecord MakeRecord(const char* zero_padded_name,
                                  std::uint32_t value_bits,
                                  std::uint8_t value_type,
                                  std::uint32_t sequence) noexcept {
  ParameterRecord record{};
  if (zero_padded_name != nullptr) {
    std::memcpy(record.name, zero_padded_name, sizeof(record.name));
  }
  record.value_bits = value_bits;
  record.sequence = sequence;
  record.magic = kRecordMagic;
  record.value_type = value_type;
  record.operation = kSetOperation;
  record.crc32 = Crc32(&record, offsetof(ParameterRecord, crc32));
  return record;
}

inline bool IsValid(const ParameterRecord& record) noexcept {
  return (record.magic == kRecordMagic) &&
         IsSupportedValueType(record.value_type) &&
         (record.operation == kSetOperation) &&
         (record.crc32 ==
          Crc32(&record, offsetof(ParameterRecord, crc32)));
}

inline bool IsSequenceNewer(std::uint32_t candidate,
                            std::uint32_t reference) noexcept {
  return static_cast<std::int32_t>(candidate - reference) > 0;
}

}  // namespace parameters::persistence_format
