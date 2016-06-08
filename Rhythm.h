#pragma once

#include "Arduino.h"
#include "Constant.h"
#include "Layer.h"

namespace Zebra {

  class Rhythm {
  private:
    uint8_t tempo;
    bool metronome;
    uint8_t bar;
    uint8_t measure;
    uint8_t load;
    uint8_t save;
    bool output;
    uint8_t quantize;
    uint32_t playTime;
    uint32_t songTime;
    Layer layerLibrary[kLayerLibrarySize];
  public:
    Rhythm();
    ~Rhythm();
    void setTempo(uint8_t tempo_);
    uint8_t getTempo() const;
    void setMetronome(bool metronome_);
    bool getMetronome() const;
    void setBar(uint8_t bar_);
    uint8_t getBar() const;
    void setMeasure(uint8_t measure_);
    uint8_t getMeasure() const;
    void setLoad(uint8_t load_);
    uint8_t getLoad() const;
    void setSave(uint8_t save_);
    uint8_t getSave() const;
    void setOutput(bool output_);
    bool getOutput() const;
    void setQuantize(uint8_t quantize_);
    uint8_t getQuantize() const;
    // play functions
    void setPlayTime(uint32_t playTime_);
    uint32_t getPlayTime() const;
    void incrementPlayTime();
    void restartPlayTime();
    void calculateSongTime();
    uint32_t getSongTime() const;
    // layer functions
    Layer& getLayer(uint8_t layerNum);
  };
}
