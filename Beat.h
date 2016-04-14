#pragma once

#include "Arduino.h"
#include "Timeframe.h"

namespace Zebra {

  const uint8_t kMaxFill = 127;

  class Beat {
  private:
    uint16_t data;    // 0 active, 0000000000 timeframe, 0 sector, 0000 free
    uint8_t fill;
  public:
    Beat();
    ~Beat();
    void reset();
    void set(uint16_t timeframe_, bool sector_, uint8_t fill_);
    void setActive(bool active_);
    bool getActive() const;
    void setTimeframe(uint16_t timeframe_);
    uint16_t getTimeframe() const;
    void setSector(bool sector_);
    bool getSector() const;
    void setFill(uint8_t fill_);
    uint8_t getFill() const;
  };
}
