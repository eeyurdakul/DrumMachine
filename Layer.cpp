#include "Layer.h"

namespace Zebra {

  Layer::Layer(uint8_t number_)
  : number(number_)
  , volume(100)
  , startY(52 + (67 * number_))
  , beatActive(true)
  , fillActive(true)
  , selectActive(false)
  , lastActiveBeat(-1)
  , beatLibrary {} {
    switch (number) {
      case 0:
      color = YELLOW;
      instA = 0;
      instB = 1;
      break;
      case 1:
      color = GREEN;
      instA = 10;
      instB = 11;
      break;
      case 2:
      color = CYAN;
      instA = 20;
      instB = 21;
      break;
      case 3:
      color = MAGENTA;
      instA = 30;
      instB = 31;
      break;
      default:
      color = WHITE;
      instA = 0;
      instB = 1;
      break;
    }
  }

  Layer::~Layer() {}

  void Layer::setNumber(uint8_t number_) {
    number = number_;
  }

  uint8_t Layer::getNumber() const {
    return number;
  }

  void Layer::setColor(uint16_t color_) {
    color = color_;
  }

  uint16_t Layer::getColor() const {
    return color;
  }

  void Layer::setVolume(uint8_t volume_) {
    volume = volume_;
  }

  uint8_t Layer::getVolume() const {
    return volume;
  }

  void Layer::setInstA(uint8_t instA_) {
    instA = instA_;
  }

  uint8_t Layer::getInstA() const {
    return instA;
  }

  void Layer::setInstB(uint8_t instB_) {
    instB = instB_;
  }

  uint8_t Layer::getInstB() const {
    return instB;
  }

  void Layer::setStartY(uint16_t startY_) {
    startY = startY_;
  }

  uint16_t Layer::getStartY() const {
    return startY;
  }

  void Layer::setBeatActive(bool beatActive_) {
    beatActive = beatActive_;
  }

  bool Layer::getBeatActive() const {
    return beatActive;
  }

  void Layer::setFillActive(bool fillActive_) {
    fillActive = fillActive_;
  }

  bool Layer::getFillActive() const {
    return fillActive;
  }

  void Layer::setSelectActive(bool selectActive_) {
    selectActive = selectActive_;
  }

  bool Layer::getSelectActive() const {
    return selectActive;
  }

  void Layer::calculateLastActiveBeat() {
    if (getBeat(0).getActive() == 0) {
      lastActiveBeat = -1;
    } else {
      for (uint8_t i = 0; i < kBeatLibrarySize; i++) {
        if (getBeat(i).getActive()) {
          lastActiveBeat = i;
        }
      }
    }
  }

  int8_t Layer::getLastActiveBeat() const {
    return lastActiveBeat;
  }

  Beat& Layer::getBeat(uint8_t beatNum) {
    return beatLibrary[beatNum];
  }
}
