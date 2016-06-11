#pragma once

#include "Arduino.h"
#include "Constant.h"
#include "Timeframe.h"

namespace Zebra {

  class Beat {
  private:
    uint16_t data;    // 0 active, 00000000000 time, 00 volume, 0 inst, 0 free
    uint8_t fill;
  public:
    Beat();
    ~Beat();
    void reset();
    void set(uint16_t time_, uint8_t volume_, bool inst_, uint8_t fill_);
    void setActive(bool active_);
    bool getActive() const;
    void setTime(uint16_t time_);
    uint32_t getTime() const;
    void setVolume(uint8_t volume_);
    uint8_t getVolume() const;
    void setInst(bool inst_);
    bool getInst() const;
    void setFill(uint8_t fill_);
    uint8_t getFill() const;
  };
}
