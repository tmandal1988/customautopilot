#include "gps_itow_deduplicator.h"

int main() {
  {
    GpsItowDeduplicator filter;

    // Zero is a valid first epoch, not an uninitialized sentinel.
    if (!filter.Accept(0U)) return 1;
    if (filter.Accept(0U)) return 2;

    if (!filter.Accept(40U)) return 3;
    if (filter.Accept(40U)) return 4;
    if (!filter.Accept(80U)) return 5;
  }

  {
    GpsItowDeduplicator filter;

    // iTOW wraps at the GPS week boundary, so a lower but different value is
    // still a fresh epoch.
    if (!filter.Accept(604799960U)) return 6;
    if (!filter.Accept(0U)) return 7;
    if (filter.Accept(0U)) return 8;
  }

  return 0;
}
