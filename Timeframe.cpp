#include "Timeframe.h"

namespace Zebra {

  Timeframe::Timeframe()
  : data(0) {}

  Timeframe::~Timeframe() {}

  // sector0 functions

  void Timeframe::resetSector0() {
    data &= ~((1 << 3) | (1 << 2) | (1 << 1) | (1 << 0));
  }

  void Timeframe::setSector0(uint8_t volume_, bool type_, bool inst_) {
    setSector0Volume(volume_);
    setSector0Type(type_);
    setSector0Inst(inst_);
  }

  void Timeframe::setSector0Volume(uint8_t volume_) {
    if (volume_ <= kMaxVolume) {
      // resetting volume
      data &= ~((1 << 3) | (1 << 2));
      // setting new volume
      data += (volume_ << 2);
    }
  }

  uint8_t Timeframe::getSector0Volume() const {
    uint8_t volume = (data >> 2);
    volume &= ~((1 << 5) | (1 << 4) | (1 << 3) | (1 << 2));
    return volume;
  }

  void Timeframe::setSector0Type(bool type_) {
    // resetting type
    data &= ~(1 << 1);
    // setting new type
    data += (type_ << 1);
  }

  bool Timeframe::getSector0Type() const {
    uint8_t type = (data >> 1);
    type &= ~((1 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1));
    return type;
  }

  void Timeframe::setSector0Inst(bool inst_) {
    // resetting inst
    data &= ~(1 << 0);
    // setting new inst
    data += inst_;
  }

  bool Timeframe::getSector0Inst() const {
    uint8_t inst = data;
    inst &= ~((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1));
    return inst;
  }

  // sector1 functions

  void Timeframe::resetSector1() {
    data &= ~((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4));
  }

  void Timeframe::setSector1(uint8_t volume_, bool type_, bool inst_) {
    setSector1Volume(volume_);
    setSector1Type(type_);
    setSector1Inst(inst_);
  }

  void Timeframe::setSector1Volume(uint8_t volume_) {
    if (volume_ <= kMaxVolume) {
      // resetting volume
      data &= ~((1 << 7) | (1 << 6));
      // setting new volume
      data += (volume_ << 6);
    }
  }

  uint8_t Timeframe::getSector1Volume() const {
    uint8_t volume = (data >> 6);
    return volume;
  }

  void Timeframe::setSector1Type(bool type_) {
    // resetting type
    data &= ~(1 << 5);
    // setting new type
    data += (type_ << 5);
  }

  bool Timeframe::getSector1Type() const {
    uint8_t type = (data >> 5);
    type &= ~((1 << 2) | (1 << 1));
    return type;
  }

  void Timeframe::setSector1Inst(bool inst_) {
    // resetting inst
    data &= ~(1 << 4);
    // setting new inst
    data += (inst_ << 4);
  }

  bool Timeframe::getSector1Inst() const {
    uint8_t inst = (data >> 4);
    inst &= ~((1 << 3) | (1 << 2) | (1 << 1));
    return inst;
  }
}
