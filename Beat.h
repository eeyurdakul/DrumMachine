#pragma once

#include "Arduino.h"
#include "Timeframe.h"

namespace Zebra {

  class Beat {
  private:
    uint16_t data;    // 0 active, 0000000000 timeframe, 0 sector, 0000 free
    uint8_t fill;
  public:
    Beat();
    ~Beat();
    void reset();
    void setActive(bool active_);
    bool getActive() const;
    void setInst(bool inst_);
    bool getInst() const;
    void setVolume(uint8_t volume_);
    uint8_t getVolume() const;
    void setTimeframe(uint16_t timeframe_);
    uint16_t getTimeframe() const;
    void setSector(bool sector_);
    bool getSector() const;
    void setFill(uint8_t fill_);
    uint8_t getFill() const;
  };
}
