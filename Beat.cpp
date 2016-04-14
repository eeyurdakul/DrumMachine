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

  void Beat::set(uint16_t timeframe_, bool sector_, uint8_t fill_) {
    setActive(true);
    setTimeframe(timeframe_);
    setSector(sector_);
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

  void Beat::setTimeframe(uint16_t timeframe_) {
    // resetting timeframe
    data &= ~(0b111111111 << 5);
    // setting new timeframe
    data += (timeframe_ << 5);
  }

  uint16_t Beat::getTimeframe() const {
    uint16_t timeframe = (data >> 5);
    timeframe &= ~(1 << 10);
    return timeframe;
  }

  void Beat::setSector(bool sector_) {
    // resetting sector
    data &= ~(1 << 4);
    // setting new sector
    data += (sector_ << 4);
  }

  bool Beat::getSector() const {
    uint8_t sector = (data >> 4);
    sector &= ~(0b11111111111 << 1);
    return sector;
  }

  void Beat::setFill(uint8_t fill_) {
    if (fill_ <= kMaxFill) {
      fill = fill_;
    }
  }

  uint8_t Beat::getFill() const {
    return fill;
  }
}
