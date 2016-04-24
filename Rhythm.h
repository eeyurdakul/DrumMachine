#pragma once

#include "Arduino.h"
#include "Constant.h"
#include "Layer.h"

namespace Zebra {

  class Rhythm {
  private:
    uint8_t tempo;
    uint8_t quantize;
    uint8_t bar;
    uint8_t measure;
    bool selectActive;
    uint32_t playTime;
    uint32_t songTime;
    Layer layerLibrary[kLayerLibrarySize];
  public:
    Rhythm();
    ~Rhythm();
    void setTempo(uint8_t tempo_);
    uint8_t getTempo() const;
    void setQuantize(uint8_t quantize_);
    uint8_t getQuantize() const;
    void setBar(uint8_t bar_);
    uint8_t getBar() const;
    void setMeasure(uint8_t measure_);
    uint8_t getMeasure() const;
    void setSelectActive(bool active_);
    bool getSelectActive() const;
    void setPlayTime(uint32_t playTime_);
    uint32_t getPlayTime() const;
    void incrementPlayTime();
    void calculateSongTime();
    uint32_t getSongTime() const;
    Layer& getLayer(uint8_t layerNum);
  };
}
