#pragma once

#include "Arduino.h"
#include "Constant.h"
#include "Layer.h"

namespace Zebra {

  class Rhythm {
  private:
    uint8_t tempo;
    bool metronome;
    bool output;
    uint8_t bar;
    uint8_t measure;
    uint8_t quantize;
    bool selected;
    bool tempoSelected;
    bool metronomeSelected;
    bool barSelected;
    bool measureSelected;
    bool loadSelected;
    bool saveSelected;
    bool outputSelected;
    bool quantizeSelected;
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
    void setOutput(bool output_);
    bool getOutput() const;
    void setBar(uint8_t bar_);
    uint8_t getBar() const;
    void setMeasure(uint8_t measure_);
    uint8_t getMeasure() const;
    void setQuantize(uint8_t quantize_);
    uint8_t getQuantize() const;
    // selection functions
    void setSelected(bool selected_);
    bool getSelected() const;
    void setTempoSelected(bool selected_);
    bool getTempoSelected() const;
    void setMetronomeSelected(bool selected_);
    bool getMetronomeSelected() const;
    void setBarSelected(bool selected_);
    bool getBarSelected() const;
    void setMeasureSelected(bool selected_);
    bool getMeasureSelected() const;
    void setLoadSelected(bool selected_);
    bool getLoadSelected() const;
    void setSaveSelected(bool selected_);
    bool getSaveSelected() const;
    void setOutputSelected(bool selected_);
    bool getOutputSelected() const;
    void setQuantizeSelected(bool selected_);
    bool getQuantizeSelected() const;
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
