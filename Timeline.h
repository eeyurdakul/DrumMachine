#pragma once

#include "Arduino.h"
#include "Timeframe.h"

namespace Zebra {

  const uint16_t kTimeframeLibrarySize = 1024;
  const uint16_t kMaxTime = 2047;

  class Timeline {
  private:
    Timeframe timeframeLibrary[kTimeframeLibrarySize];
  public:
    Timeline();
    ~Timeline();
    void reset(uint16_t time_);
    void set(uint16_t time_, uint8_t volume_, bool type_, bool inst_);
    void setVolume(uint16_t time_, uint8_t volume_);
    uint8_t getVolume(uint16_t time_) const;
    void setType(uint16_t time_, bool type_);    // 0 -> beat, 1 -> fill
    bool getType(uint16_t time_) const;
    void setInst(uint16_t time_, bool inst_);
    bool getInst(uint16_t time_) const;
  };
}
