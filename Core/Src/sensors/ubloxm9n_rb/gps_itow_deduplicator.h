#pragma once

#include <cstdint>

// Filters repeated u-blox navigation epochs at the sensor boundary. iTOW is
// allowed to move backward (for example, at the GPS week rollover), so only an
// exact repeat of the last accepted epoch is considered a duplicate.
class GpsItowDeduplicator final {
 public:
  bool Accept(std::uint32_t i_tow) noexcept {
    if (has_last_i_tow_ && (i_tow == last_i_tow_)) {
      return false;
    }

    last_i_tow_ = i_tow;
    has_last_i_tow_ = true;
    return true;
  }

 private:
  std::uint32_t last_i_tow_ = 0U;
  bool has_last_i_tow_ = false;
};
