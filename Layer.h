#pragma once

#include "Arduino.h"
#include "Color.h"
#include "Timeline.h"
#include "Beat.h"

namespace Zebra {

  const uint8_t kMinLayerVolume = 0;
  const uint8_t kMaxLayerVolume = 10;
  const uint8_t kMinLayerInst = 0;
  const uint8_t kMaxLayerInst = 127;
  const uint8_t kBeatLibrarySize = 32;

  class Layer {
  private:
    uint8_t number;
    uint16_t color;
    uint8_t volume;
    uint8_t instA;
    uint8_t instB;
    uint16_t startY;
    bool beatActive;
    bool fillActive;
    bool selectActive;
    int8_t lastActiveBeat;
    Timeline timeline;
    Beat beatLibrary[kBeatLibrarySize];

  public:
    Layer(uint8_t number_);
    ~Layer();
    void setNumber(uint8_t number_);
    uint8_t getNumber() const;
    void setColor(uint16_t color_);
    uint16_t getColor() const;
    void setVolume(uint8_t volume_);
    uint8_t getVolume() const;
    void setInstA(uint8_t instA_);
    uint8_t getInstA() const;
    void setInstB(uint8_t instB_);
    uint8_t getInstB() const;
    void setStartY(uint16_t startY_);
    uint16_t getStartY() const;
    void setBeatActive(bool beatActive_);
    bool getBeatActive() const;
    void setFillActive(bool fillActive_);
    bool getFillActive() const;
    void setSelectActive(bool selectActive_);
    bool getSelectActive() const;
    void calculateLastActiveBeat();
    int8_t getLastActiveBeat() const;
    Beat& getBeat(uint8_t beatNum);
    void setBeat(uint16_t time_, uint8_t volume_, bool inst_);
    void setFill(uint8_t beatNum, uint8_t fill);
    void clearBeat(uint8_t beatNum);
    void reset();
  };
}
