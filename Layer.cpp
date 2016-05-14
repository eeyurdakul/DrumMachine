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

  // private fill functions

  uint8_t Layer::getFillStep(uint8_t fillNum) {
    return pgm_read_byte(&fillStepLibrary[fillNum]);
  }

  char Layer::getFillName(uint8_t fillNum, uint8_t letterNum) {
    unsigned int flashAddress = pgm_read_word(&fillNameLibrary[fillNum]);
    return (char) pgm_read_byte(flashAddress + letterNum);
  }

  uint8_t Layer::getFillTime(uint8_t fillNum, uint8_t stepNum) {
    unsigned int flashAddress = pgm_read_word(&fillTimeLibrary[fillNum]);
    return pgm_read_byte(flashAddress + stepNum);
  }

  uint8_t Layer::getFillVolume(uint8_t fillNum, uint8_t stepNum) {
    unsigned int flashAddress = pgm_read_word(&fillVolumeLibrary[fillNum]);
    return pgm_read_byte(flashAddress + stepNum);
  }

  uint8_t Layer::getFillInst(uint8_t fillNum, uint8_t stepNum) {
    unsigned int flashAddress = pgm_read_word(&fillInstLibrary[fillNum]);
    return pgm_read_byte(flashAddress + stepNum);
  }

  // public functions

  void Layer::reset() {
    // resetting beat library
    for (uint8_t i = 0; i < kBeatLibrarySize; i++) {
      getBeat(i).reset();
    }
    // resetting timeline
    for (uint16_t j = 0; j < kMaxTime + 1; j++) {
      timeline.reset(j);
    }
    // calculating last active beat
    calculateLastActiveBeat();
  }

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

  Timeline& Layer::getTimeline() {
    return timeline;
  }

  uint8_t Layer::setBeat(uint16_t time_, uint8_t volume_, bool inst_) {
    uint8_t beatNum;
    bool shift;
    // checking if given data is correct
    if ((time_ <= kMaxTime) && (volume_ <= kMaxVolume) && (inst_ <= kMaxVolume)) {
      // checking right beatNum for given time
      if (getLastActiveBeat() < kBeatLibrarySize - 1) {
        for (uint8_t i = 0; i < kBeatLibrarySize; i++) {
          beatNum = i;
          if (getBeat(beatNum).getActive() == 0) {
            shift = false;
            break;
          } else if (time_ < getBeat(beatNum).getTime()) {
            shift = true;
            break;
          }
        }
        // shifting beats right if necessary
        if (shift) {
          for (uint8_t j = kBeatLibrarySize - 1; j > beatNum; j--) {
            getBeat(j) = getBeat(j - 1);
          }
        }
        // resetting beat
        getBeat(beatNum).reset();
        // setting beat
        getBeat(beatNum).set(time_, volume_, inst_, 0);
        // clearing previous beat's fill
        if (beatNum > 0) {
          // resetting fill of previous beat
          getBeat(beatNum - 1).setFill(0);
          // getting previous beat data
          uint16_t previousBeatTime = getBeat(beatNum - 1).getTime();
          uint16_t nextBeatTime;
          bool previousBeatInst = timeline.getInst(previousBeatTime);
          uint8_t previousBeatVolume = timeline.getVolume(previousBeatTime);
          // calculating next beat time
          // checking if this beat is last beat
          if (beatNum == (kBeatLibrarySize - 1)) {
            nextBeatTime = kMaxTime + 1;
          } else
          // checking if next beat is not active
          if (getBeat(beatNum + 1).getActive() == 0) {
            nextBeatTime = kMaxTime + 1;
          } else {
            nextBeatTime = getBeat(beatNum + 1).getTime();
          }
          // clearing previous beat's fill in timeline
          for (uint16_t k = previousBeatTime; k < nextBeatTime; k++) {
            timeline.reset(k);
          }
          // re-setting previous beat in timeline
          timeline.set(previousBeatTime, previousBeatVolume, 0, previousBeatInst);
        }
        // setting current beat in timeline
        timeline.set(time_, volume_, 0, inst_);
        // calculating last active beat
        calculateLastActiveBeat();
        return beatNum;
      }
    }
  }

  void Layer::setFill(uint8_t beatNum, uint8_t fillNum) {
    // checking if given data is correct
    if ((beatNum < kBeatLibrarySize) && (fillNum < kFillLibrarySize)) {
      // changing beat's fill
      getBeat(beatNum).setFill(fillNum);
      // resetting fill's area in timeline
      uint16_t thisBeatTime = getBeat(beatNum).getTime();
      uint16_t nextBeatTime;
      // checking if this beat is last beat
      if (beatNum == (kBeatLibrarySize - 1)) {
        nextBeatTime = kMaxTime + 1;
      } else
      // checking if next beat is not active
      if (getBeat(beatNum + 1).getActive() == 0) {
        nextBeatTime = kMaxTime + 1;
      } else {
        nextBeatTime = getBeat(beatNum + 1).getTime();
      }
      // resetting timeline
      for (uint16_t i = thisBeatTime; i < nextBeatTime; i++) {
        timeline.reset(i);
      }
      // setting fill in timeline
      uint8_t fillStep = getFillStep(fillNum);
      uint32_t fillTotalTime = nextBeatTime - thisBeatTime;
      uint16_t fillTotalRefTime = 0;
      float fillRefTime = 0;
      // calculating fillTotalRefTime
      for (uint8_t j = 0; j < fillStep + 1; j++) {
        fillTotalRefTime += getFillTime(fillNum, j);
      }
      // calculating and setting fill data into timeline
      for (uint8_t k = 0; k < fillStep; k++) {
        fillRefTime += getFillTime(fillNum, k);
        uint16_t fillTime = thisBeatTime + (fillTotalTime * fillRefTime / fillTotalRefTime);
        uint8_t volume = getFillVolume(fillNum, k);
        bool type = 1;
        bool inst = getFillInst(fillNum, k);
        timeline.set(fillTime, volume, type, inst);
        // Serial.println(fillRefTime);
        // Serial.println(fillTime);
        // Serial.println(volume);
      }
    }
  }

  void Layer::clearBeat(uint8_t beatNum) {
    // checking if given data is correct
    if (beatNum < kBeatLibrarySize) {
      // resetting beat
      uint16_t currentBeatTime = getBeat(beatNum).getTime();
      uint16_t nextBeatTime;
      uint16_t previousBeatTime;
      getBeat(beatNum).reset();
      // checking if shifting left is necessary
      bool shift;
      if (beatNum == kBeatLibrarySize - 1) {
        shift = false;
      } else if (beatNum == getLastActiveBeat()) {
        shift = false;
      } else {
        shift = true;
      }
      // shifting beats left if necessary
      if (shift) {
        for (uint8_t i = beatNum; i < kBeatLibrarySize - 2; i++) {
          getBeat(i) = getBeat(i + 1);
        }
        getBeat(kBeatLibrarySize - 1).reset();
      }
      // clearing timeline from previous beat till next beat
      if (beatNum > 0) {
        // resetting fill of previous beat
        getBeat(beatNum - 1).setFill(0);
        // getting previous beat data
        previousBeatTime = getBeat(beatNum - 1).getTime();
        bool previousBeatInst = timeline.getInst(previousBeatTime);
        uint8_t previousBeatVolume = timeline.getVolume(previousBeatTime);
        // calculating next beat time
        // checking if this beat is last beat
        if (beatNum == (kBeatLibrarySize - 1)) {
          nextBeatTime = kMaxTime + 1;
        } else
        // checking if next beat is not active
        if (getBeat(beatNum + 1).getActive() == 0) {
          nextBeatTime = kMaxTime + 1;
        } else {
          nextBeatTime = getBeat(beatNum + 1).getTime();
        }
        // clearing previous beat's fill in timeline
        for (uint16_t k = previousBeatTime; k < nextBeatTime; k++) {
          timeline.reset(k);
        }
        // re-setting previous beat in timeline
        timeline.set(previousBeatTime, previousBeatVolume, 0, previousBeatInst);
      } else if (beatNum == 0) {
        nextBeatTime = getBeat(beatNum + 1).getTime();
        // clearing beat's fill in timeline
        for (uint16_t l = currentBeatTime; l < nextBeatTime; l++) {
          timeline.reset(l);
        }
      }
      // calculating last active beat
      calculateLastActiveBeat();
    }
  }
}
