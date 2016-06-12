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
    bool fillEditMode;
    int8_t selectedBeatNum;
    int8_t currentRhythmMenu;
    int8_t previousRhythmMenu;
    int8_t currentLayerMenu;
    int8_t previousLayerMenu;
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
    void checkKeyboard();
    // play functions
    void record();
    void playStop();
    void reset();
    // select functions
    void rhythmSelect();
    void layerSelect(Layer& layer_);
    void layerChannelSelect(Layer& layer_);
    // global select function
    void select();
    // menu function
    void menuUpdate();
    // rhythm menu functions
    void rhythmMenuRight();
    void rhythmMenuLeft();
    void rhythmMenuUp();
    void rhythmMenuDown();
    // rhythm functions
    void tempoUp();
    void tempoDown();
    void metronomeUp();
    void metronomeDown();
    void barUp();
    void barDown();
    void measureUp();
    void measureDown();
    void loadUp();
    void loadDown();
    void saveUp();
    void saveDown();
    void outputUp();
    void outputDown();
    void quantizeUp();
    void quantizeDown();
    // layer menu functions
    void layerMenuRight();
    void layerMenuLeft();
    void layerMenuUp();
    void layerMenuDown();
    // layer functions
    void beatUp();
    void beatDown();
    void fillUp();
    void fillDown();
    void instAUp();
    void instADown();
    void instBUp();
    void instBDown();
    // beat record functions
    void beatA();
    void beatB();
    void beatClear();
  };
}
