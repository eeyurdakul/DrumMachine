#include "Beat.h"

namespace Zebra {

  Beat::Beat()
  : data(0)
  , fill(0) {}

  Beat::~Beat() {}

  void Beat::reset() {
    data = 0;
    fill = 0;
  }

  void Beat::set(uint16_t time_, uint8_t fill_) {
    setActive(true);
    setTime(time_);
    setFill(fill_);
  }

  void Beat::setActive(bool active_) {
    // resetting state
    data &= ~(1 << 15);
    // setting new state
    data += (active_ << 15);
  }

  bool Beat::getActive() const {
    bool active = (data >> 15);
    return active;
  }

  void Beat::setTime(uint16_t time_) {
    // resetting timeframe
    data &= ~(0b11111111111 << 4);
    // setting new timeframe
    data += (time_ << 4);
  }

  uint16_t Beat::getTime() const {
    uint16_t time = (data >> 4);
    time &= ~(1 << 11);
    return time;
  }

  void Beat::setFill(uint8_t fill_) {
    if (fill_ < kFillLibrarySize) {
      fill = fill_;
    }
  }

  uint8_t Beat::getFill() const {
    return fill;
  }
}
