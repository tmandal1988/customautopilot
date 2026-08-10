/*
 * log_file_name.h
 *
 *  Created on: Aug 5, 2026
 *      Author: tanmay
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

// Naming rules for the per-power-cycle log files. This is deliberately free of
// FatFs and FreeRTOS dependencies so the index allocation can be exercised on
// a host without an SD card attached.
namespace log_file_name {

// Log files are named LOG_nnnn.BIN. That spelling is uppercase and 8.3 legal,
// so FatFs stores it without an accompanying long-name entry and f_readdir
// returns exactly the string this code wrote. Index 0 is never allocated,
// which lets 0 stand for "no log file selected".
inline constexpr std::uint32_t kMaxIndex = 9999U;
inline constexpr std::size_t kNameLength = 12U;  // strlen("LOG_0001.BIN")
inline constexpr std::size_t kNameBufferSize = kNameLength + 1U;

inline char UpperAscii(char value) noexcept {
  return ((value >= 'a') && (value <= 'z'))
             ? static_cast<char>(value - ('a' - 'A'))
             : value;
}

inline bool EqualsIgnoreCaseAscii(const char* lhs, const char* rhs) noexcept {
  if ((lhs == nullptr) || (rhs == nullptr)) {
    return false;
  }
  std::size_t index = 0U;
  while ((lhs[index] != '\0') && (rhs[index] != '\0')) {
    if (UpperAscii(lhs[index]) != UpperAscii(rhs[index])) {
      return false;
    }
    ++index;
  }
  return (lhs[index] == '\0') && (rhs[index] == '\0');
}

// Writes "LOG_nnnn.BIN" for index into out_buffer, which must hold at least
// kNameBufferSize bytes.
//
// Indices are allocated by NextIndex(), which never yields more than
// kMaxIndex, so the wrap below is unreachable by construction. It is kept so
// the function is total: without it an out-of-range index would be truncated
// by snprintf into a shorter name that no longer round-trips through
// ParseIndex(), which is a far quieter failure than a wrapped one.
inline void Format(std::uint32_t index, char* out_buffer) noexcept {
  const unsigned long printable =
      static_cast<unsigned long>(index % (kMaxIndex + 1U));
  std::snprintf(out_buffer, kNameBufferSize, "LOG_%04lu.BIN", printable);
}

// Recovers the index from a directory entry, or returns false if the name is
// not one this firmware generated. Only the exact LOG_nnnn.BIN shape is
// accepted, so unrelated files on the card cannot influence which index is
// allocated next. The comparison is case-insensitive because a log copied
// back onto the card from a host may return from f_readdir as a lowercase
// long name, and FAT would still refuse to create the uppercase spelling
// alongside it.
inline bool ParseIndex(const char* name, std::uint32_t* index) noexcept {
  if ((name == nullptr) || (index == nullptr)) {
    return false;
  }
  if (std::strlen(name) != kNameLength) {
    return false;
  }
  static constexpr char kPattern[] = "LOG_nnnn.BIN";
  std::uint32_t value = 0U;
  for (std::size_t position = 0U; position < kNameLength; ++position) {
    const char actual = name[position];
    if (kPattern[position] == 'n') {
      if ((actual < '0') || (actual > '9')) {
        return false;
      }
      value = (value * 10U) + static_cast<std::uint32_t>(actual - '0');
    } else if (UpperAscii(actual) != kPattern[position]) {
      return false;
    }
  }
  *index = value;
  return true;
}

// Chooses the index for a new log file given the highest one already on the
// card (0 when none exist). Returns false once the space is exhausted.
//
// Rollover deliberately fails instead of wrapping back to index 1. Wrapping
// would reintroduce exactly the failure this naming scheme exists to prevent:
// silently destroying a log that has never been downloaded. Deleting logs
// frees indices and is the intended recovery.
inline bool NextIndex(std::uint32_t highest_existing,
                      std::uint32_t* next_index) noexcept {
  if ((next_index == nullptr) || (highest_existing >= kMaxIndex)) {
    return false;
  }
  *next_index = highest_existing + 1U;
  return true;
}

}  // namespace log_file_name
