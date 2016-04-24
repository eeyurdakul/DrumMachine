#pragma once

#include <Adafruit_TFTLCD.h>
#include "Arduino.h"
#include <avr/pgmspace.h>
#include "Constant.h"
#include "Rhythm.h"
#include "Digit.h"
#include "Fill.h"

#define LCD_RD    A0
#define LCD_WR    A1
#define LCD_CD    A2
#define LCD_CS    A3
#define LCD_RST   A4

namespace Zebra {

  class View: public Adafruit_TFTLCD {
  private:
    Rhythm & rhythmRef;
    float barX;
    float measureX;
    uint16_t playX;
    uint16_t playColor;
    float playXRatio;
    uint8_t rhythmTempoClean;
    uint8_t rhythmQuantizeClean;
    uint8_t rhythmBarClean;
    uint8_t rhythmMeasureClean;
    uint8_t layerInstAClean;
    uint8_t layerInstBClean;
    int8_t fillNumberClean;
    uint16_t selectedFillXStartClean;
    uint16_t selectedFillXEndClean;
    uint16_t selectedFillYClean;
    // private info functions
    void drawInfoNumber(uint8_t num, uint8_t digit, uint16_t xPos, uint16_t yPos, uint16_t color);
    void drawInfoLetter(char letter, uint16_t xPos, uint16_t yPos, uint16_t color);
    void drawInfoDigit(uint8_t digit, uint16_t xPos, uint16_t yPos, uint16_t color);
    // private fill functions
    uint8_t getFillStep(uint8_t fillNum);
    char getFillName(uint8_t fillNum, uint8_t letterNum);
    uint8_t getFillTime(uint8_t fillNum, uint8_t stepNum);
    uint8_t getFillVolume(uint8_t fillNum, uint8_t stepNum);
    uint8_t getFillInst(uint8_t fillNum, uint8_t stepNum);
  public:
    View(Rhythm& rhythm);
    ~View();
    void initialize();
    // play functions
    void setPlayX(uint16_t playX_);
    uint16_t getPlayX() const;
    void drawPlayBar();
    void restartPlayBar();
    void resetPlayBar();
    void calculatePlayXRatio();
    // rhythm functions
    void drawRhythmSelectActive();
    void drawSelectedFill(uint8_t selectedLayer_, uint8_t selectedBeat_);
    // layer functions
    void drawLayerAll(Layer& layer_);
    void drawLayerBase(const Layer& layer_);
    void drawLayerSelectActive(const Layer& layer_);
    void drawLayerBeatActive(const Layer& layer_);
    void drawLayerFillActive(const Layer& layer_);
    void drawLayerMeasure(const Layer& layer_);
    void drawLayerSong(Layer& layer_);
    void drawAllLayer();
    void drawAllLayerMeasure();
    void drawAllLayerMeasureAndSong();
    // info rhythm functions
    void drawInfoRhythmAll();
    void drawInfoRhythmBase();
    void drawInfoRhythmTempo();
    void drawInfoRhythmQuantize();
    void drawInfoRhythmBar();
    void drawInfoRhythmMeasure();
    void cleanInfoRhythm();
    // info layer functions
    void drawInfoLayerAll(const Layer& layer_, const Beat& beat_);
    void drawInfoLayerBase();
    void drawInfoLayerInstA(const Layer& layer_);
    void drawInfoLayerInstB(const Layer& layer_);
    void drawInfoFill(const Beat& beat_);
    void cleanInfoFill();
    // debug functions
    void debug(uint32_t var);
  };
}
