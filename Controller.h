#pragma once

#include "Arduino.h"
#include "Constant.h"
#include "Rhythm.h"
#include "View.h"
#include "Keyboard.h"
#include "Fill.h"

namespace Zebra {

  class Controller {
  private:
    Rhythm& rhythmRef;
    View& viewRef;
    Keyboard keyboard;
    uint8_t selectedLayer;
    uint8_t selectedBeat;
    // private timing functions
    void adjustBarUpTiming();
    void adjustBarDownTiming();
    void adjustMeasureUpTiming();
    void adjustMeasureDownTiming();
    void adjustTiming();
    // private fill functions
    uint8_t getFillStep(uint8_t fillNum);
    char getFillName(uint8_t fillNum, uint8_t letterNum);
    uint8_t getFillTime(uint8_t fillNum, uint8_t stepNum);
    uint8_t getFillVolume(uint8_t fillNum, uint8_t stepNum);
    uint8_t getFillInst(uint8_t fillNum, uint8_t stepNum);
  public:
    Controller(Rhythm& rhythm_, View& view_);
    ~Controller();
    void initialize();
    void setSelectedLayer(uint8_t selectedLayer_);
    uint8_t getSelectedLayer() const;
    void setSelectedBeat(uint8_t selectedBeat_);
    uint8_t getSelectedBeat() const;
    void checkKeyboardStatus();
    // play functions
    void playButtonPressed();
    void resetButtonPressed();
    // select functions
    void rhythmSelectButtonPressed();
    void layerSelectButtonPressed(Layer& layer_);
    void layerBeatButtonPressed(Layer& layer_);
    void layerFillButtonPressed(Layer& layer_);
    // rhythm functions
    void rhythmTempoUpButtonPressed();
    void rhythmTempoDownButtonPressed();
    void rhythmQuantizeUpButtonPressed();
    void rhythmQuantizeDownButtonPressed();
    void rhythmBarUpButtonPressed();
    void rhythmBarDownButtonPressed();
    void rhythmMeasureUpButtonPressed();
    void rhythmMeasureDownButtonPressed();
    // layer functions
    void layerInstAUpButtonPressed();
    void layerInstADownButtonPressed();
    void layerInstBUpButtonPressed();
    void layerInstBDownButtonPressed();
    // fill functions
    void beatUpButtonPressed();
    void beatDownButtonPressed();
    void fillUpButtonPressed();
    void fillDownButtonPressed();
  };
}
