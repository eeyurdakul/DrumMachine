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

  void Beat::setInst(bool inst_) {
    // resetting inst
    data &= ~(1 << 14);
    // setting new inst
    data += (inst_ << 14);
  }

  bool Beat::getInst() const {
    uint8_t inst = (data >> 14);
    inst &= ~(1 << 1);
    return inst;
  }

  void Beat::setVolume(uint8_t volume_) {
    if (volume_ <= kMaxVolume) {
      // resetting volume
      data &= ~((1 << 13) | (1 << 12));
      // setting new volume
      data += (volume_ << 12);
    }
  }

  uint8_t Beat::getVolume() const {
    uint8_t volume = (data >> 12);
    volume &= ~((1 << 3) | (1 << 2));
    return volume;
  }

  void Beat::setTimeframe(uint16_t timeframe_) {
    if (timeframe_ <= kMaxTimeframe) {
      // resetting timeframe
      data &= ~(0b111111111 << 2);
      // setting new timeframe
      data += (timeframe_ << 2);
    }
  }

  uint16_t Beat::getTimeframe() const {
    uint16_t timeframe = (data >> 2);
    timeframe &= ~(0b1111 << 10);
    return timeframe;
  }

  void Beat::setSector(bool sector_) {
    // resetting sector
    data &= ~(1 << 1);
    // setting new sector
    data += (sector_ << 1);
  }

  bool Beat::getSector() const {
    uint8_t sector = (data >> 1);
    sector &= ~(0b11111111111111 << 1);
    return sector;
  }

  void Beat::setFill(uint8_t fill_) {
    fill = fill_;
  }

  uint8_t Beat::getFill() const {
    return fill;
  }
}
