#pragma once

#include "Arduino.h"
#include "Constant.h"

namespace Zebra {

  class Midi {
  private:
    uint8_t channel;
  public:
    Midi();
    ~Midi();
    void initialize();
    void setChannel(uint8_t channel_);
    uint8_t getChannel() const;
    void noteOn(uint8_t pitch, uint8_t velocity);
    void noteOff(uint8_t pitch, uint8_t velocity);
  };
}
