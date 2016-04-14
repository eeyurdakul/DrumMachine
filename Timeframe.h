#pragma once

#include "Arduino.h"

namespace Zebra {

  const uint8_t kMaxVolume = 3;   // 0 -> passive, 1 -> 30, 2 -> 60, 3 -> 90
  const uint16_t kMaxTimeframe = 1023;
  const uint8_t kMaxFill = 127;

  class Timeframe {
  private:
    uint8_t data;  // 00 volume, 0 type, 0 inst // 00 volume, 0 type, 0 inst
  public:
    Timeframe();
    ~Timeframe();
  };
}
