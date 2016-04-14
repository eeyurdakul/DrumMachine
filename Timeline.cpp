#include "Timeline.h"

namespace Zebra {
  Timeline::Timeline() {}

  Timeline::~Timeline() {}

  void Timeline::reset(uint16_t time_) {
    if (time_ <= kMaxTime) {
      uint16_t timeframe = time_ / 2;
      bool sector = time_ % 2;
      timeframeLibrary[timeframe].reset(sector);
    }
  }

  void Timeline::set(uint16_t time_, uint8_t volume_, bool type_, bool inst_) {
    if (time_ <= kMaxTime) {
      uint16_t timeframe = time_ / 2;
      bool sector = time_ % 2;
      timeframeLibrary[timeframe].set(sector, volume_, type_, inst_);
    }
  }

  void Timeline::setVolume(uint16_t time_, uint8_t volume_) {
    if (time_ <= kMaxTime) {
      uint16_t timeframe = time_ / 2;
      bool sector = time_ % 2;
      timeframeLibrary[timeframe].setVolume(sector, volume_);
    }
  }

  uint8_t Timeline::getVolume(uint16_t time_) const {
    if (time_ <= kMaxTime) {
      uint16_t timeframe = time_ / 2;
      bool sector = time_ % 2;
      timeframeLibrary[timeframe].getVolume(sector);
    }
  }

  void Timeline::setType(uint16_t time_, bool type_) {
    if (time_ <= kMaxTime) {
      uint16_t timeframe = time_ / 2;
      bool sector = time_ % 2;
      timeframeLibrary[timeframe].setType(sector, type_);
    }
  }

  bool Timeline::getType(uint16_t time_) const {
    if (time_ <= kMaxTime) {
      uint16_t timeframe = time_ / 2;
      bool sector = time_ % 2;
      timeframeLibrary[timeframe].getType(sector);
    }
  }

  void Timeline::setInst(uint16_t time_, bool inst_) {
    if (time_ <= kMaxTime) {
      uint16_t timeframe = time_ / 2;
      bool sector = time_ % 2;
      timeframeLibrary[timeframe].setInst(sector, inst_);
    }
  }

  bool Timeline::getInst(uint16_t time_) const {
    if (time_ <= kMaxTime) {
      uint16_t timeframe = time_ / 2;
      bool sector = time_ % 2;
      timeframeLibrary[timeframe].getInst(sector);
    }
  }
}
