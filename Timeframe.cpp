#include "Timeframe.h"

namespace Zebra {

  Timeframe::Timeframe()
  : data(0) {}

  Timeframe::~Timeframe() {}

  // sector0 functions

  void Timeframe::reset(bool sector_) {
    if (sector_) {
      data &= ~((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0));
    } else {
      data &= ~((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4));
    }
  }

  void Timeframe::set(bool sector_, uint8_t volume_, bool type_, bool inst_) {
    setVolume(sector_, volume_);
    setType(sector_, type_);
    setInst(sector_, inst_);
  }

  void Timeframe::setVolume(bool sector_, uint8_t volume_) {
    if (volume_ <= kMaxVolume) {
      if (sector_) {
        // resetting volume
        data &= ~((1 << 3) | (1 << 2));
        // setting new volume
        data += (volume_ << 2);
      } else {
        // resetting volume
        data &= ~((1 << 7) | (1 << 6));
        // setting new volume
        data += (volume_ << 6);
      }
    }
  }

  uint8_t Timeframe::getVolume(bool sector_) const {
    uint8_t volume;
    if (sector_) {
      volume = (data >> 2);
      volume &= ~((1 << 5) | (1 << 4) | (1 << 3) | (1 << 2));
    } else {
      volume = (data >> 6);
    }
    return volume;
  }

  void Timeframe::setType(bool sector_, bool type_) {
    if (sector_) {
      // resetting type
      data &= ~(1 << 1);
      // setting new type
      data += (type_ << 1);
    } else {
      // resetting type
      data &= ~(1 << 5);
      // setting new type
      data += (type_ << 5);
    }
  }

  bool Timeframe::getType(bool sector_) const {
    uint8_t type;
    if (sector_) {
      type = (data >> 1);
      type &= ~((1 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1));
    } else {
      type = (data >> 5);
      type &= ~((1 << 2) | (1 << 1));
    }
    return type;
  }

  void Timeframe::setInst(bool sector_, bool inst_) {
    if (sector_) {
      // resetting inst
      data &= ~(1 << 0);
      // setting new inst
      data += inst_;
    } else {
      // resetting inst
      data &= ~(1 << 4);
      // setting new inst
      data += (inst_ << 4);
    }
  }

  bool Timeframe::getInst(bool sector_) const {
    uint8_t inst;
    if (sector_) {
      inst = data;
      inst &= ~((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1));
    } else {
      inst = (data >> 4);
      inst &= ~((1 << 3) | (1 << 2) | (1 << 1));
    }
    return inst;
  }
}
