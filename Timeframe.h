#pragma once

#include "Arduino.h"

namespace Zebra {

  const uint8_t kMaxVolume = 3;   // 0 -> passive, 1 -> 30, 2 -> 60, 3 -> 90
  const uint16_t kMaxTimeframe = 1023;

  class Timeframe {
  private:
    uint8_t data;  // 00 volume, 0 type, 0 inst // 00 volume, 0 type, 0 inst
  public:
    Timeframe();
    ~Timeframe();
    // Sector0 functions
    void resetSector0();
    void setSector0(uint8_t volume_, bool type_, bool inst_);
    void setSector0Volume(uint8_t volume_);
    uint8_t getSector0Volume() const;
    void setSector0Type(bool type_);
    bool getSector0Type() const;
    void setSector0Inst(bool inst_);
    bool getSector0Inst() const;
    // Sector1 functions
    void resetSector1();
    void setSector1(uint8_t volume_, bool type_, bool inst_);
    void setSector1Volume(uint8_t volume_);
    uint8_t getSector1Volume() const;
    void setSector0Type(bool type_);
    bool getSector0Type() const;
    void setSector0Inst(bool inst_);
    bool getSector0Inst() const;
  };
}
