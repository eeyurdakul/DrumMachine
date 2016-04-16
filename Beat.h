#pragma once

#include "Arduino.h"
#include "Constant.h"
#include "Timeframe.h"

namespace Zebra {

  class Beat {
  private:
    uint16_t data;    // 0 active, 00000000000 time, 0000 free
    uint8_t fill;
  public:
    Beat();
    ~Beat();
    void reset();
    void set(uint16_t time_, uint8_t fill_);
    void setActive(bool active_);
    bool getActive() const;
    void setTime(uint16_t time_);
    uint16_t getTime() const;
    void setFill(uint8_t fill_);
    uint8_t getFill() const;
  };
}
