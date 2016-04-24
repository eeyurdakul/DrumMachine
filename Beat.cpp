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

  void Beat::set(uint16_t time_, uint8_t volume_, bool inst_, uint8_t fill_) {
    setActive(true);
    setTime(time_);
    setVolume(volume_);
    setInst(inst_);
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

  uint32_t Beat::getTime() const {
    uint32_t time = (data >> 4);
    time &= ~(1 << 11);
    return time;
  }

  void Beat::setVolume(uint8_t volume_) {
    if (volume_ <= kMaxVolume) {
      // resetting volume
      data &= ~((1 << 3) | (1 << 2));
      // setting new volume
      data += (volume_ << 2);
    }
  }

  uint8_t Beat::getVolume() const {
    uint16_t volume = (data >> 2);
    volume &= ~(0b111111111111 << 2);
    return volume;
  }

  void Beat::setInst(bool inst_) {
    // resetting inst
    data &= ~(1 << 1);
    // setting new inst
    data += (inst_ << 1);
  }

  bool Beat::getInst() const {
    uint16_t inst = (data >> 1);
    inst &= ~(0b11111111111111 << 1);
    return inst;
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
