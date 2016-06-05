#pragma once

#include "Arduino.h"
#include "Constant.h"
#include "Timeline.h"
#include "Beat.h"
#include "Fill.h"

namespace Zebra {

  class Layer {
  private:
    uint8_t number;
    uint16_t color;
    uint8_t volume;
    uint8_t instAMidi;
    uint8_t instBMidi;
    uint8_t instAAudio;
    uint8_t instBAudio;
    uint16_t startY;
    bool beatActive;
    bool fillActive;
    bool selected;
    int8_t lastActiveBeat;
    Timeline timeline;
    Beat beatLibrary[kBeatLibrarySize];
    // private fill functions
    uint8_t getFillStep(uint8_t fillNum);
    char getFillName(uint8_t fillNum, uint8_t letterNum);
    uint8_t getFillTime(uint8_t fillNum, uint8_t stepNum);
    uint8_t getFillVolume(uint8_t fillNum, uint8_t stepNum);
    uint8_t getFillInst(uint8_t fillNum, uint8_t stepNum);
  public:
    Layer(uint8_t number_);
    ~Layer();
    void reset();
    void setNumber(uint8_t number_);
    uint8_t getNumber() const;
    void setColor(uint16_t color_);
    uint16_t getColor() const;
    void setVolume(uint8_t volume_);
    uint8_t getVolume() const;
    void setInstAMidi(uint8_t instA_);
    uint8_t getInstAMidi() const;
    void setInstBMidi(uint8_t instB_);
    uint8_t getInstBMidi() const;
    void setInstAAudio(uint8_t instA_);
    uint8_t getInstAAudio() const;
    void setInstBAudio(uint8_t instB_);
    uint8_t getInstBAudio() const;
    void setStartY(uint16_t startY_);
    uint16_t getStartY() const;
    void setBeatActive(bool beatActive_);
    bool getBeatActive() const;
    void setFillActive(bool fillActive_);
    bool getFillActive() const;
    void setSelected(bool selected_);
    bool getSelected() const;
    void calculateLastActiveBeat();
    int8_t getLastActiveBeat() const;
    Beat& getBeat(uint8_t beatNum);
    Timeline& getTimeline();
    uint8_t setBeat(uint16_t time_, uint8_t volume_, bool inst_);
    void setFill(uint8_t beatNum, uint8_t fillNum);
    void clearBeat(uint8_t beatNum);
  };
}
