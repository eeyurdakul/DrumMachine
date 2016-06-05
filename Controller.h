#pragma once

#include "Arduino.h"
#include "Constant.h"
#include "Rhythm.h"
#include "View.h"
#include "Player.h"
#include "Keyboard.h"
#include "Fill.h"

namespace Zebra {

  class Controller {
  private:
    Rhythm& rhythmRef;
    View& viewRef;
    Player& playerRef;
    Keyboard keyboard;
    Layer* selectedLayer;
    Beat* selectedBeat;
    int8_t selectedBeatNum;
    // private timing functions
    void adjustBarUpTiming();
    void adjustBarDownTiming();
    void adjustMeasureUpTiming();
    void adjustMeasureDownTiming();
    // private fill functions
    uint8_t getFillStep(uint8_t fillNum);
    char getFillName(uint8_t fillNum, uint8_t letterNum);
    uint8_t getFillTime(uint8_t fillNum, uint8_t stepNum);
    uint8_t getFillVolume(uint8_t fillNum, uint8_t stepNum);
    uint8_t getFillInst(uint8_t fillNum, uint8_t stepNum);
  public:
    Controller(Rhythm& rhythm_, View& view_, Player& player_);
    ~Controller();
    void initialize();
    void checkKeyboardStatus();
    // play functions
    void recordButtonPressed();
    void playButtonPressed();
    void resetButtonPressed();
    // select functions
    void rhythmSelectButtonPressed();
    void layerSelectButtonPressed(Layer& layer_);
    void layerBeatButtonPressed(Layer& layer_);
    void layerFillButtonPressed(Layer& layer_);
    // rhythm functions
    void rhythmMoveRightButtonPressed();
    void rhythmMoveLeftButtonPressed();
    void rhythmTempoUpButtonPressed();
    void rhythmTempoDownButtonPressed();
    void rhythmMetronomeUpButtonPressed();
    void rhythmMetronomeDownButtonPressed();
    void rhythmBarUpButtonPressed();
    void rhythmBarDownButtonPressed();
    void rhythmMeasureUpButtonPressed();
    void rhythmMeasureDownButtonPressed();
    void rhythmLoadUpButtonPressed();
    void rhythmLoadDownButtonPressed();
    void rhythmSaveUpButtonPressed();
    void rhythmSaveDownButtonPressed();
    void rhythmOutputUpButtonPressed();
    void rhythmOutputDownButtonPressed();
    void rhythmQuantizeUpButtonPressed();
    void rhythmQuantizeDownButtonPressed();
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
    // beat record functions
    void beatAButtonPressed();
    void beatBButtonPressed();
    void beatClearButtonPressed();
    // metronome functions
    void metronomeButtonPressed();
  };
}
