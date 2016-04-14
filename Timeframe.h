#pragma once

#include "Arduino.h"

namespace Zebra {

  const uint8_t kMaxVolume = 3;   // 0 -> passive, 1 -> 30, 2 -> 60, 3 -> 90

  class Timeframe {
  private:
    uint8_t data;  // 00 volume, 0 type, 0 inst // 00 volume, 0 type, 0 inst
  public:
    Timeframe();
    ~Timeframe();
    void reset(bool sector_);
    void set(bool sector_, uint8_t volume_, bool type_, bool inst_);
    void setVolume(bool sector_, uint8_t volume_);
    uint8_t getVolume(bool sector_) const;
    void setType(bool sector_, bool type_);    // 0 -> beat, 1 -> fill
    bool getType(bool sector_) const;
    void setInst(bool sector_, bool inst_);
    bool getInst(bool sector_) const;
  };
}
