#pragma once

#include "Arduino.h"
#include "Constant.h"
#include "Timeframe.h"

namespace Zebra {

  class Timeline {
  private:
    Timeframe timeframeLibrary[kTimeframeLibrarySize];
  public:
    Timeline();
    ~Timeline();
    void reset(uint32_t time_);
    void set(uint32_t time_, uint8_t volume_, bool type_, bool inst_);
    void setVolume(uint32_t time_, uint8_t volume_);
    uint8_t getVolume(uint32_t time_) const;
    void setType(uint32_t time_, bool type_);    // 0 -> beat, 1 -> fill
    bool getType(uint32_t time_) const;
    void setInst(uint32_t time_, bool inst_);
    bool getInst(uint32_t time_) const;
  };
}
