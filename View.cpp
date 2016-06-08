#include "View.h"

namespace Zebra {

  View::View(Rhythm& rhythm_)
  : Adafruit_TFTLCD(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RST)
  , rhythmRef(rhythm_)
  , barX(0)
  , measureX(0)
  , playX(0)
  , measurePlayCount(0)
  , playColor(kPlayColor0)
  // rhythm menu variables
  , tempoClean(0)
  , metronomeClean(0)
  , barClean(0)
  , measureClean(0)
  , loadClean(0)
  , saveClean(0)
  , outputClean(0)
  , quantizeClean(0)
  // layer menu variables
  , instAClean(0)
  , instBClean(0)
  , fillNumberClean(255) {}

  View::~View() {}

  void View::initialize() {
    reset();
    uint16_t identifier = readID();
    if (identifier == 0x0101)
    identifier = 0x9341;
    begin(identifier);
    setRotation(3);
    setTextSize(1);
    fillScreen(BLACK);
    calculatePlayXRatio();
  }

  void View::update() {
    drawPlayBar();
  }

  // play functions

  void View::setPlayX(uint16_t playX_) {
    playX = playX_;
  }

  uint16_t View::getPlayX() const {
    return playX;
  }

  void View::drawPlayBar() {
    if ((rhythmRef.getPlayTime() / playXRatio) > playX) {
      drawPixel(kSongStartX + playX, kPlayY, playColor);
      playX += 1;
    }
  }

  void View::restartPlayBar() {
    playX = 0;
    measurePlayCount = 0;
    if (playColor == kPlayColor0) {
      drawPixel(kSongEndX, kPlayY, playColor);
      playColor = kPlayColor1;
    } else {
      drawPixel(kSongEndX, kPlayY, playColor);
      playColor = kPlayColor0;
    }
  }

  void View::resetPlayBar() {
    drawFastHLine(kSongStartX, kPlayY, kSongX + 1, kPlayColor1);
    drawFastHLine(kSongStartX, kPlayY + 1, kSongX + 1, kPlayColor1);
    playX = 0;
    measurePlayCount = 0;
    playColor = kPlayColor0;
  }

  void View::calculatePlayXRatio() {
    playXRatio = float(rhythmRef.getSongTime()) / kSongX;
  }

  // layer functions

  void View::drawLayerAll(Layer& layer_) {
    drawLayerBase(layer_);
    drawLayerBeatActive(layer_);
    drawLayerFillActive(layer_);
    drawLayerSelected(layer_);
    drawLayerMeasure(layer_);
    drawLayerSong(layer_);
    drawLayerFill(layer_);
  }

  void View::drawLayerBase(const Layer& layer_) {
    if (&layer_ != NULL) {
      const uint16_t kLayerBaseYPos = 66;
      drawFastHLine(10, layer_.getStartY() + kLayerBaseYPos, 460, layer_.getColor());
      fillRect(15, layer_.getStartY() + 29, 7, 7, layer_.getColor());
    }
  }

  void View::drawLayerSelected(const Layer& layer_) {
    if (&layer_ != NULL) {
      uint16_t color;
      if (layer_.getSelected()) {
        color = layer_.getColor();
      } else {
        color = BLACK;
      }
      fillRect(0, layer_.getStartY() + 29, 15, 7, color);
    }
  }

  void View::drawLayerBeatActive(const Layer& layer_) {
    if (&layer_ != NULL) {
      const uint16_t kLayerBeatActiveXPos = 435;
      const uint16_t kLayerBeatActiveYPos = 29;
      if (layer_.getBeatActive()) {
        setTextColor(layer_.getColor());
      } else {
        setTextColor(DGRAY);
      }
      setCursor(kLayerBeatActiveXPos, layer_.getStartY() + kLayerBeatActiveYPos);
      println(F("B"));
    }
  }

  void View::drawLayerFillActive(const Layer& layer_) {
    if (&layer_ != NULL) {
      const uint16_t kLayerFillActiveXPos = 455;
      const uint16_t kLayerFillActiveYPos = 29;
      if (layer_.getFillActive()) {
        setTextColor(layer_.getColor());
      } else {
        setTextColor(DGRAY);
      }
      setCursor(kLayerFillActiveXPos, layer_.getStartY() + kLayerFillActiveYPos);
      println(F("F"));
    }
  }

  void View::drawLayerMeasure(const Layer& layer_) {
    if (&layer_ != NULL) {
      // clearing measure
      fillRect(kSongStartX, layer_.getStartY() + 45, kSongX, 12, BLACK);
      drawFastHLine(kSongStartX, layer_.getStartY() + 56, kSongX + 1, LGRAY);
      barX = kSongX / rhythmRef.getBar();
      uint16_t segment = rhythmRef.getMeasure() * rhythmRef.getBar();
      measureX = float(kSongX) / segment;
      for (uint8_t i = 0; i <= segment; i ++) {
        uint16_t posX = kSongStartX + int(i * measureX);
        if (i % rhythmRef.getMeasure() == 0) {
          drawFastVLine(posX, layer_.getStartY() + 46, 6, LGRAY);
        } else {
          drawFastVLine(posX, layer_.getStartY() + 50, 2, LGRAY);
        }
      }
    }
  }

  void View::drawLayerSong(Layer& layer_) {
    if (&layer_ != NULL) {
      fillRect(kSongStartX, layer_.getStartY() + 23, kSongX + 1, 13, BLACK);
      for (uint8_t i = 0; i <= layer_.getLastActiveBeat(); i++) {
        Beat& beat = layer_.getBeat(i);
        if (beat.getActive()) {
          uint16_t xPos = kSongStartX + (kSongX  * beat.getTime() / rhythmRef.getSongTime());
          uint16_t yPos = layer_.getStartY() + 23;
          if (!beat.getInst()) {
            drawFastVLine(xPos, yPos, 13, layer_.getColor());
          } else {
            for (uint8_t i = 0; i < 5; i++) {
              drawPixel(xPos, yPos + (3 * i), layer_.getColor());
            }
          }
        }
      }
    }
  }

  void View::drawLayerBeat(const Layer& layer_, uint32_t time_, bool inst_) {
    if (&layer_ != NULL) {
      uint16_t xPos = kSongStartX + (kSongX * time_ / rhythmRef.getSongTime());
      uint16_t yPos = layer_.getStartY() + 23;
      if (!inst_) {
        drawFastVLine(xPos, yPos, 13, layer_.getColor());
      } else {
        for (uint8_t i = 0; i < 5; i++) {
          drawPixel(xPos, yPos + (3 * i), layer_.getColor());
        }
      }
    }
  }

  void View::clearLayerBeat(const Layer& layer_, const Beat& beat_) {
    if ((&layer_ != NULL) && (&beat_ != NULL)) {
      uint32_t beatTime = beat_.getTime();
      bool beatInst = beat_.getInst();
      uint16_t xPos = kSongStartX + (kSongX * beatTime / rhythmRef.getSongTime());
      uint16_t yPos = layer_.getStartY() + 23;
      drawFastVLine(xPos, yPos, 13, BLACK);
    }
  }

  void View::drawBeatFill(Layer& layer_, uint8_t beatNum_, bool condition) {
    if (&layer_ != NULL) {
      // introducing time variables
      uint32_t beatTime;
      uint32_t nextBeatTime;
      uint32_t loopBeatTime;
      bool loop = false;
      // introducing position variables
      uint16_t xStartPos;
      uint16_t xEndPos;
      uint16_t xLoopStartPos;
      uint16_t xLoopEndPos;
      uint16_t yPos;
      uint16_t color;
      // calculating starting fill time
      beatTime = layer_.getBeat(beatNum_).getTime();
      // calculating ending fill time
      if (beatNum_ == kBeatLibrarySize - 1) {
        nextBeatTime = rhythmRef.getSongTime();
        loop = true;
      } else if (!layer_.getBeat(beatNum_ + 1).getActive()) {
        nextBeatTime = rhythmRef.getSongTime();
        loop = true;
      } else {
        nextBeatTime = layer_.getBeat(beatNum_ + 1).getTime();
      }
      // calculating starting X position
      xStartPos = kSongStartX + (beatTime * kSongX / rhythmRef.getSongTime()) + 1;
      // calculating ending X position
      xEndPos = kSongStartX + (nextBeatTime * kSongX / rhythmRef.getSongTime()) - 1;
      // calculating loop X position
      if (loop) {
        loopBeatTime = layer_.getBeat(0).getTime();
        xLoopStartPos = kSongStartX;
        xLoopEndPos = kSongStartX + (loopBeatTime * kSongX / rhythmRef.getSongTime()) - 1;
      }
      // calculating Y position
      yPos = layer_.getStartY() + 38;
      // drawing fill
      if (condition) {
        color = layer_.getColor();
      } else {
        color = BLACK;
      }
      drawFastHLine(xStartPos, yPos, xEndPos - xStartPos + 1, color);
      if (loop) {
        drawFastHLine(xLoopStartPos, yPos, xLoopEndPos - xLoopStartPos + 1, color);
      }
    }
  }

  void View::drawLayerFill(Layer& layer_) {
    if (&layer_ != NULL) {
      // calculating Y position
      uint16_t yPos = layer_.getStartY() + 38;
      // clearing old fill line
      drawFastHLine(kSongStartX, yPos, kSongX, BLACK);
      for (uint8_t i = 0; i <= layer_.getLastActiveBeat(); i++) {
        drawBeatFill(layer_, i, layer_.getBeat(i).getFill());
      }
    }
  }

  void View::drawAllLayer() {
    for (uint8_t i = 0; i < kLayerLibrarySize; i++) {
      drawLayerAll(rhythmRef.getLayer(i));
    }
  }

  void View::drawAllLayerMeasure() {
    for (uint8_t i = 0; i < kLayerLibrarySize; i++) {
      drawLayerMeasure(rhythmRef.getLayer(i));
    }
  }

  void View::drawAllLayerMeasureSongFill() {
    for (uint8_t i = 0; i < kLayerLibrarySize; i++) {
      drawLayerMeasure(rhythmRef.getLayer(i));
      drawLayerSong(rhythmRef.getLayer(i));
      drawLayerFill(rhythmRef.getLayer(i));
    }
  }

  void View::drawSelectedBeat(const Layer& layer_, const Beat& beat_) {
    uint16_t xPos = selectedBeatXClean;
    uint16_t yPos = selectedBeatYClean;
    fillTriangle(xPos, yPos, xPos - 3, yPos - 3, xPos + 3, yPos - 3, BLACK);
    // checking if layer is active and has at least one beat
    if ((&layer_ != NULL) && (&beat_ != NULL)) {
      // calculating starting beat time
      uint32_t beatTime = beat_.getTime();
      // calculating starting X position
      xPos = kSongStartX + (beatTime * kSongX / rhythmRef.getSongTime());
      // calculating Y position
      yPos = layer_.getStartY() + 15;
      // drawing selected fill
      fillTriangle(xPos, yPos, xPos - 3, yPos - 3, xPos + 3, yPos - 3, layer_.getColor());
      // saving X - Y positions to clean state
      selectedBeatXClean = xPos;
      //selectedFillXEndClean = xEndPos;
      selectedBeatYClean = yPos;
    }
  }

  // rhythm menu functions

  void View::drawRhythmMenuBox(uint8_t boxNum, bool state) {
    uint16_t backColor;
    uint16_t foreColor;
    uint16_t boxXPos = kRhythmMenuBoxXPos[boxNum];
    uint16_t boxYPos = kRhythmMenuBoxYPos;
    uint16_t headerXPos = kRhythmMenuHeaderXPos[boxNum];
    uint16_t headerYPos = kRhythmMenuHeaderYPos;
    uint8_t dataDigit = kRhythmMenuDataDigit[boxNum];
    uint8_t dataXPos = kRhythmMenuDataXPos[boxNum];
    uint8_t dataYPos = kRhythmMenuDataYPos;
    uint8_t data;
    uint8_t* dataClean;
    // defining colors
    menuColorSelect(backColor, foreColor, state);
    // drawing box
    fillRect(boxXPos, boxYPos, kRhythmMenuBoxWidth, kRhythmMenuBoxHeight, backColor);
    // drawing header
    setTextColor(foreColor);
    setCursor(headerXPos, headerYPos);
    switch (boxNum) {
      case 0: // tempo
      println(F("TEMPO"));
      data = rhythmRef.getTempo();
      dataClean = &tempoClean;
      break;
      case 1: // metronome
      println(F("METRONOME"));
      data = rhythmRef.getMetronome();
      dataClean = &metronomeClean;
      break;
      case 2: // bar
      println(F("BAR"));
      data = rhythmRef.getBar();
      dataClean = &barClean;
      break;
      case 3: // measure
      println(F("MEASURE"));
      data = rhythmRef.getMeasure();
      dataClean = &measureClean;
      break;
      case 4: // load
      println(F("LOAD"));
      data = rhythmRef.getLoad();
      dataClean = &loadClean;
      break;
      case 5: // save
      println(F("SAVE"));
      data = rhythmRef.getSave();
      dataClean = &saveClean;
      break;
      case 6: // output
      println(F("OUTPUT"));
      data = rhythmRef.getOutput();
      dataClean = &outputClean;
      break;
      case 7: // quantize
      println(F("QUANTIZE"));
      data = rhythmRef.getQuantize();
      dataClean = &quantizeClean;
      break;
    }
    drawMenuNumber(data, dataDigit, dataXPos, dataYPos, foreColor);
    *dataClean = data;
  }

  void View::drawRhythmMenuData(uint8_t boxNum) {
    uint16_t backColor = BLACK;
    uint16_t foreColor = WHITE;
    uint8_t dataDigit = kRhythmMenuDataDigit[boxNum];
    uint8_t dataXPos = kRhythmMenuDataXPos[boxNum];
    uint8_t dataYPos = kRhythmMenuDataYPos;
    uint8_t *dataClean;
    uint8_t data;
    switch (boxNum) {
      case 0: // tempo
      data = rhythmRef.getTempo();
      dataClean = &tempoClean;
      break;
      case 1: // metronome
      data = rhythmRef.getMetronome();
      dataClean = &metronomeClean;
      break;
      case 2: // bar
      data = rhythmRef.getBar();
      dataClean = &barClean;
      break;
      case 3: // measure
      data = rhythmRef.getMeasure();
      dataClean = &measureClean;
      break;
      case 4: // load
      data = rhythmRef.getLoad();
      dataClean = &loadClean;
      break;
      case 5: // save
      data = rhythmRef.getSave();
      dataClean = &saveClean;
      break;
      case 6: // output
      data = rhythmRef.getOutput();
      dataClean = &outputClean;
      break;
      case 7: // quantize
      data = rhythmRef.getQuantize();
      dataClean = &quantizeClean;
      break;
    }
    drawMenuNumber(*dataClean, dataDigit, dataXPos, dataYPos, backColor);
    drawMenuNumber(data, dataDigit, dataXPos, dataYPos, foreColor);
    *dataClean = data;
  }

  // layer menu functions

  void View::drawLayerMenuBox(uint8_t boxNum, bool state, const Layer& layer_) {
    uint16_t backColor;
    uint16_t foreColor;
    uint16_t boxXPos = kLayerMenuBoxXPos[boxNum];
    uint16_t boxYPos = kLayerMenuBoxYPos;
    uint16_t headerXPos = kLayerMenuHeaderXPos[boxNum];
    uint16_t headerYPos = kLayerMenuHeaderYPos;
    uint8_t dataDigit = kLayerMenuDataDigit[boxNum];
    uint8_t dataXPos = kLayerMenuDataXPos[boxNum];
    uint8_t dataYPos = kLayerMenuDataYPos;
    uint8_t data;
    uint8_t* dataClean;
    // defining colors
    menuColorSelect(backColor, foreColor, state);
    // drawing box
    fillRect(boxXPos, boxYPos, kLayerMenuBoxWidth, kLayerMenuBoxHeight, backColor);
    // drawing header
    setTextColor(foreColor);
    setCursor(headerXPos, headerYPos);
    switch (boxNum) {
      case 0: // fill
      // statement
      break;
      case 1: // measure
      println(F("INST A"));
      data = layer_.getInstAMidi();
      dataClean = &instAClean;
      break;
      case 2: // load
      println(F("INST B"));
      data = layer_.getInstBMidi();
      dataClean = &instBClean;
      break;
      default:
      break;
    }
    drawMenuNumber(data, dataDigit, dataXPos, dataYPos, foreColor);
    *dataClean = data;
  }

  void View::drawLayerMenuData(uint8_t boxNum, const Layer& layer_) {
    uint16_t backColor = BLACK;
    uint16_t foreColor = WHITE;
    uint8_t dataDigit = kLayerMenuDataDigit[boxNum];
    uint8_t dataXPos = kLayerMenuDataXPos[boxNum];
    uint8_t dataYPos = kLayerMenuDataYPos;
    uint8_t *dataClean;
    uint8_t data;
    switch (boxNum) {
      case 0: // fill
      // statement
      break;
      case 1: // tempo
      data = layer_.getInstAMidi();
      dataClean = &instAClean;
      break;
      case 2: // metronome
      data = layer_.getInstBMidi();
      dataClean = &instBClean;
      break;
      default:
      break;
    }
    drawMenuNumber(*dataClean, dataDigit, dataXPos, dataYPos, backColor);
    drawMenuNumber(data, dataDigit, dataXPos, dataYPos, foreColor);
    *dataClean = data;
  }

  // private menu functions

  void View::menuColorSelect(uint16_t& backColor_, uint16_t& foreColor_, bool state) {
    if (state) {
      backColor_ = BLACK;
      foreColor_ = LGRAY;
    } else {
      backColor_ = WHITE;
      foreColor_ = BLACK;
    }
  }

  void View::drawMenuNumber(uint8_t num, uint8_t digit, uint16_t xPos, uint16_t yPos, uint16_t color) {
    uint8_t digitHundreds;
    uint8_t digitTens;
    uint8_t digitOnes;

    if ((digit >= kMinMenuDigit) && (digit <= kMaxMenuDigit)) {
      if (digit == 3) {
        digitHundreds = (num / 100);
        drawMenuDigit(digitHundreds, xPos, yPos, color);
        xPos += kMenuDigitOffset;
      }
      if (digit >= 2) {
        digitTens = (num % 100) / 10;
        drawMenuDigit(digitTens, xPos, yPos, color);
        xPos += kMenuDigitOffset;
      }
      digitOnes = num % 10;
      drawMenuDigit(digitOnes, xPos, yPos, color);
    }
  }

  void View::drawMenuLetter(char letter, uint16_t xPos, uint16_t yPos, uint16_t color) {
    uint8_t letterDigit = 0;
    switch (letter) {
      case '0':
      letterDigit = 0;
      break;
      case '1':
      letterDigit = 1;
      break;
      case '2':
      letterDigit = 2;
      break;
      case '3':
      letterDigit = 3;
      break;
      case '4':
      letterDigit = 4;
      break;
      case '5':
      letterDigit = 5;
      break;
      case '6':
      letterDigit = 6;
      break;
      case '7':
      letterDigit = 7;
      break;
      case '8':
      letterDigit = 8;
      break;
      case '9':
      letterDigit = 9;
      break;
      case 'A':
      case 'a':
      letterDigit = 10;
      break;
      case 'B':
      case 'b':
      letterDigit = 11;
      break;
      case 'C':
      case 'c':
      letterDigit = 12;
      break;
      case 'D':
      case 'd':
      letterDigit = 13;
      break;
      case 'E':
      case 'e':
      letterDigit = 14;
      break;
      case 'F':
      case 'f':
      letterDigit = 15;
      break;
      case 'G':
      case 'g':
      letterDigit = 16;
      break;
      case 'H':
      case 'h':
      letterDigit = 17;
      break;
      case 'I':
      case 'i':
      letterDigit = 18;
      break;
      case 'J':
      case 'j':
      letterDigit = 19;
      break;
      case 'K':
      case 'k':
      letterDigit = 20;
      break;
      case 'L':
      case 'l':
      letterDigit = 21;
      break;
      case 'M':
      case 'm':
      letterDigit = 22;
      break;
      case 'N':
      case 'n':
      letterDigit = 23;
      break;
      case 'O':
      case 'o':
      letterDigit = 24;
      break;
      case 'P':
      case 'p':
      letterDigit = 25;
      break;
      case 'Q':
      case 'q':
      letterDigit = 26;
      break;
      case 'R':
      case 'r':
      letterDigit = 27;
      break;
      case 'S':
      case 's':
      letterDigit = 28;
      break;
      case 'T':
      case 't':
      letterDigit = 29;
      break;
      case 'U':
      case 'u':
      letterDigit = 30;
      break;
      case 'V':
      case 'v':
      letterDigit = 31;
      break;
      case 'W':
      case 'w':
      letterDigit = 32;
      break;
      case 'X':
      case 'x':
      letterDigit = 33;
      break;
      case 'Y':
      case 'y':
      letterDigit = 34;
      break;
      case 'Z':
      case 'z':
      letterDigit = 35;
      break;
      case '/':
      letterDigit = 36;
      break;
      case '-':
      letterDigit = 37;
      break;
      case '+':
      letterDigit = 38;
      break;
      case '%':
      letterDigit = 39;
      break;
      case ' ':
      letterDigit = 40;
      break;
      default:
      letterDigit = 40;
      break;
    }
    drawMenuDigit(letterDigit, xPos, yPos, color);
  }

  void View::drawMenuDigit(uint8_t digit, uint16_t xPos, uint16_t yPos, uint16_t color) {
    for (int y = 0; y < 13; y++) {
      for (int x = 0; x < 9; x++) {
        if (pgm_read_word(&digitFont[digit][y]) & (1 << 8 - x)) {
          drawPixel(xPos + x, yPos + y, color);
        }
      }
    }
  }

  // private fill functions

  uint8_t View::getFillStep(uint8_t fillNum) {
    return pgm_read_byte(&fillStepLibrary[fillNum]);
  }

  char View::getFillName(uint8_t fillNum, uint8_t letterNum) {
    unsigned int flashAddress = pgm_read_word(&fillNameLibrary[fillNum]);
    return (char) pgm_read_byte(flashAddress + letterNum);
  }

  uint8_t View::getFillTime(uint8_t fillNum, uint8_t stepNum) {
    unsigned int flashAddress = pgm_read_word(&fillTimeLibrary[fillNum]);
    return pgm_read_byte(flashAddress + stepNum);
  }

  uint8_t View::getFillVolume(uint8_t fillNum, uint8_t stepNum) {
    unsigned int flashAddress = pgm_read_word(&fillVolumeLibrary[fillNum]);
    return pgm_read_byte(flashAddress + stepNum);
  }

  uint8_t View::getFillInst(uint8_t fillNum, uint8_t stepNum) {
    unsigned int flashAddress = pgm_read_word(&fillInstLibrary[fillNum]);
    return pgm_read_byte(flashAddress + stepNum);
  }

  // debug functions

  void View::debug(uint32_t var) {
    fillRect(40, 260, 200, 20, BLACK);
    setTextColor(WHITE);
    setCursor(50, 265);
    print(var);
  }
}

/*

void View::drawInfoLayerAll(const Layer& layer_, const Beat& beat_) {
drawInfoLayerInstA(layer_);
drawInfoLayerInstB(layer_);
if (layer_.getLastActiveBeat() != -1) {
drawInfoFill(beat_);
} else {
cleanInfoFill();
}
}

void View::drawInfoLayerBase() {
// clearing info base
fillRect(0, 0, 480, kInfoHeight, WHITE);
// drawing section segments
drawFastVLine(240, 0, 20, BLACK);
drawFastVLine(360, 0, 20, BLACK);
// drawing fill graph base
//drawFastVLine(330, 9, 36, BLACK);
drawFastHLine(90, 45, 140, BLACK);
for(int i = 0; i < 12; i ++) {
drawPixel(230, 9 + (3 * i), BLACK);
}
setCursor(8, 8);
println(F("FILL"));
setTextColor(BLACK);
setCursor(248, 8);
println(F("INST A"));
setCursor(368, 8);
println(F("INST B"));
}

void View::drawInfoLayerInstB(const Layer& layer_) {
if (&layer_ != NULL) {
uint8_t instB = layer_.getInstBMidi();
// checking if there is a new inst data
if ((switchInfoToLayerFlag) || (instB != instBClean)) {
drawInfoNumber(instBClean, kInstBDigit, kInstBXPos, kInstBYPos, WHITE);
drawInfoNumber(instB, kInstBDigit, kInstBXPos, kInstBYPos, BLACK);
instBClean = instB;
}
}
}

void View::drawInfoFill(const Beat& beat_) {
if (&beat_ != NULL) {
// checking if there is a new fill data
if ((switchInfoToLayerFlag) || (beat_.getFill() != fillNumberClean)) {
// clearing old fill name
fillRect(kFillNameXPos, kFillNameYPos, 57, 13, WHITE);
// drawing fill name
uint16_t fillNameXPos = kFillNameXPos;
uint16_t fillNameYPos = kFillNameYPos;
for(uint8_t i = 0; i < kFillNameLetterCount; i++) {
drawInfoLetter(getFillName(beat_.getFill(), i), fillNameXPos, fillNameYPos, BLACK);
fillNameXPos += kInfoDigitOffset;
}
// clearing old fill diagram
fillRect(90, 9, 120, 36, WHITE);
// drawing fill diagram
uint8_t fill = beat_.getFill();
uint8_t step = getFillStep(fill);
uint8_t quantaTotalTimeRatio = 0;
uint8_t quantaTimeRatio = 0;
uint8_t quantaVolume = 0;
// calculating quanta total time ratio
for(int j = 0; j < step + 1; j++) {
quantaTotalTimeRatio += getFillTime(fill, j);
}
// drawing quanta time line
for(int k = 0; k < step; k++) {
quantaTimeRatio += getFillTime(fill, k);
quantaVolume = getFillVolume(fill, k);
uint16_t fillDiagXPos = int(90 + 120 * double(quantaTimeRatio) / quantaTotalTimeRatio);
float vLine = 36 * float(quantaVolume) / kMaxVolume;
drawFastVLine(fillDiagXPos, 45 - vLine, vLine, BLACK);
}
// adjusting fillNumberClean
fillNumberClean = beat_.getFill();
}
}
}

void View::cleanInfoFill() {
// checking if there is a new fill data
if ((switchInfoToLayerFlag) || (fillNumberClean != -1)) {
// clearing old fill name
fillRect(kFillNameXPos, kFillNameYPos, 57, 13, WHITE);
// clearing old fill diagram
fillRect(90, 9, 120, 36, WHITE);
uint16_t fillNameXPos = kFillNameXPos;
uint16_t fillNameYPos = kFillNameYPos;
// drawing empty fill name (hypen)
for(uint8_t i = 0; i < kFillNameLetterCount; i++) {
drawInfoLetter('-', fillNameXPos, fillNameYPos, BLACK);
fillNameXPos += kInfoDigitOffset;
}
fillNumberClean = -1;
}
}

*/
