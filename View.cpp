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
  , tempoClean(0)
  , metronomeClean(0)
  , barClean(0)
  , measureClean(0)
  , loadClean(0)
  , saveClean(0)
  , outputClean(0)
  , quantizeClean(0)
  , instAClean(0)
  , instBClean(0)
  , fillNumberClean(127) {}

  View::~View() {}

  void View::initialize() {
    reset();
    uint16_t identifier = readID();
    begin(0x9341);
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
      if (playX > (measureX * measurePlayCount)) {
        drawFastVLine(kSongStartX + playX, kPlayY, 2, playColor);
        measurePlayCount++;
      }
      playX += 1;
    }
  }

  void View::restartPlayBar() {
    playX = 0;
    measurePlayCount = 0;
    if (playColor == kPlayColor0) {
      drawFastVLine(kSongEndX, kPlayY, 2, playColor);
      playColor = kPlayColor1;
    } else {
      drawFastVLine(kSongEndX, kPlayY, 2, playColor);
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
      drawFastHLine(kSongStartX, layer_.getStartY() + 56, kSongX + 1, MGRAY);
      barX = kSongX / rhythmRef.getBar();
      uint16_t segment = rhythmRef.getMeasure() * rhythmRef.getBar();
      measureX = float(kSongX) / segment;
      for (uint8_t i = 0; i <= segment; i ++) {
        uint16_t posX = kSongStartX + int(i * measureX);
        if (i % rhythmRef.getMeasure() == 0) {
          drawFastVLine(posX, layer_.getStartY() + 46, 6, MGRAY);
        } else {
          drawFastVLine(posX, layer_.getStartY() + 50, 2, MGRAY);
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

  void View::drawRhythmMenu1() {
    fillRect(0, 0, 480, kMenuHeight, WHITE);
    drawFastVLine(120, 0, 45, BLACK);
    drawFastVLine(240, 0, 45, BLACK);
    drawFastVLine(360, 0, 45, BLACK);
    setTextColor(BLACK);
    setCursor(8, 8);
    println(F("TEMPO"));
    setCursor(128, 8);
    println(F("METRONOME"));
    setCursor(248, 8);
    println(F("BAR"));
    setCursor(368, 8);
    println(F("MEASURE"));
    drawTempo();
    drawMetronome();
    drawBar();
    drawMeasure();
  }

  void View::drawRhythmMenu2() {
    fillRect(0, 0, 480, kMenuHeight, WHITE);
    drawFastVLine(120, 0, 45, BLACK);
    drawFastVLine(240, 0, 45, BLACK);
    drawFastVLine(360, 0, 45, BLACK);
    setTextColor(BLACK);
    setCursor(8, 8);
    println(F("LOAD"));
    setCursor(128, 8);
    println(F("SAVE"));
    setCursor(248, 8);
    println(F("OUTPUT"));
    setCursor(368, 8);
    println(F("QUANTIZE"));
    drawLoad();
    drawSave();
    drawOutput();
    drawQuantize();
  }

  void View::drawRhythmMenuSelection(int8_t currentState, int8_t previousState) {
    uint16_t xPos1;
    uint16_t xPos2;
    // clearing previous state
    if (previousState >= 0) {
      switch (previousState) {
        case 0:
        case 4:
        xPos1 = 8;
        xPos2 = 20;
        break;
        case 1:
        case 5:
        xPos1 = 128;
        xPos2 = 140;
        break;
        case 2:
        case 6:
        xPos1 = 248;
        xPos2 = 260;
        break;
        case 3:
        case 7:
        xPos1 = 368;
        xPos2 = 380;
        break;
        default:
        break;
      }
      drawTriangle(xPos1, 30, xPos1, 42, xPos2, 36, WHITE);
    }
    // drawing current state
    if (currentState >= 0) {
      switch (currentState) {
        case 0:
        case 4:
        xPos1 = 8;
        xPos2 = 20;
        break;
        case 1:
        case 5:
        xPos1 = 128;
        xPos2 = 140;
        break;
        case 2:
        case 6:
        xPos1 = 248;
        xPos2 = 260;
        break;
        case 3:
        case 7:
        xPos1 = 368;
        xPos2 = 380;
        break;
        default:
        break;
      }
      drawTriangle(xPos1, 30, xPos1, 42, xPos2, 36, DGRAY);
    }
  }

  void View::drawTempo() {
    drawMenuNumber(tempoClean, kTempoDigit, kTempoXPos, kDataYPos, WHITE);
    drawMenuNumber(rhythmRef.getTempo(), kTempoDigit, kTempoXPos, kDataYPos, BLACK);
    tempoClean = rhythmRef.getTempo();
  }

  void View::drawMetronome() {
    uint16_t xPos = kMetronomeXPos;
    uint16_t yPos = kDataYPos;
    bool data;
    // clearing old state
    fillRect(xPos, yPos,33, 13, WHITE);
    // getting new state
    if (rhythmRef.getMetronome()) {
      data = 1;
    } else {
      data = 0;
    }
    // drawing new state
    for(uint8_t i = 0; i < kMetronomeDigit; i++) {
      drawMenuLetter(kMetronomeData[data][i], xPos, yPos, BLACK);
      xPos += kMenuDigitOffset;
    }
  }

  void View::drawBar() {
    drawMenuNumber(barClean, kBarDigit, kBarXPos, kDataYPos, WHITE);
    drawMenuNumber(rhythmRef.getBar(), kBarDigit, kBarXPos, kDataYPos, BLACK);
    barClean = rhythmRef.getBar();
  }

  void View::drawMeasure() {
    drawMenuNumber(measureClean, kMeasureDigit, kMeasureXPos, kDataYPos, WHITE);
    drawMenuNumber(rhythmRef.getMeasure(), kMeasureDigit, kMeasureXPos, kDataYPos, BLACK);
    measureClean = rhythmRef.getMeasure();
  }

  void View::drawLoad() {
    drawMenuNumber(loadClean, kLoadDigit, kLoadXPos, kDataYPos, WHITE);
    drawMenuNumber(rhythmRef.getLoad(), kLoadDigit, kLoadXPos, kDataYPos, BLACK);
    loadClean = rhythmRef.getLoad();
  }

  void View::drawSave() {
    drawMenuNumber(saveClean, kSaveDigit, kSaveXPos, kDataYPos, WHITE);
    drawMenuNumber(rhythmRef.getSave(), kSaveDigit, kSaveXPos, kDataYPos, BLACK);
    saveClean = rhythmRef.getSave();
  }

  void View::drawOutput() {
    uint16_t xPos = kOutputXPos;
    uint16_t yPos = kDataYPos;
    bool data;
    // clearing old state
    fillRect(xPos, yPos,45, 13, WHITE);
    // getting new state
    if (rhythmRef.getOutput()) {
      data = 1;
    } else {
      data = 0;
    }
    // drawing new state
    for(uint8_t i = 0; i < kOutputDigit; i++) {
      drawMenuLetter(kOutputData[data][i], xPos, yPos, BLACK);
      xPos += kMenuDigitOffset;
    }
  }

  void View::drawQuantize() {
    drawMenuNumber(quantizeClean, kQuantizeDigit, kQuantizeXPos, kDataYPos, WHITE);
    drawMenuNumber(pgm_read_word(&kQuantizeLibrary[rhythmRef.getQuantize()]), kQuantizeDigit, kQuantizeXPos, kDataYPos, BLACK);
    quantizeClean = pgm_read_word(&kQuantizeLibrary[rhythmRef.getQuantize()]);
  }

  // layer menu functions

  void View::drawLayerMenu(const Layer& layer_, const Beat& beat_) {
    fillRect(0, 0, 480, kMenuHeight, WHITE);
    drawFastVLine(240, 0, 45, BLACK);
    drawFastVLine(360, 0, 45, BLACK);
    drawFastHLine(kFillGraphStartXPos, kDataYPos + kFillGraphHeight, kFillGraphLength, BLACK);
    drawFastVLine(kFillGraphEndXPos, kDataYPos, 13, BLACK);
    setTextColor(BLACK);
    setCursor(8, 8);
    println(F("FILL"));
    setCursor(248, 8);
    println(F("INST A"));
    setCursor(368, 8);
    println(F("INST B"));
    drawInstA(layer_);
    drawInstB(layer_);
    if (layer_.getLastActiveBeat() != -1) {
      drawFill(beat_);
    } else {
      cleanFill();
    }
  }

  void View::drawLayerMenuSelection(int8_t currentState, int8_t previousState) {
    uint16_t xPos1;
    uint16_t xPos2;
    // clearing previous state
    if (previousState >= 0) {
      switch (previousState) {
        case 0:
        xPos1 = 8;
        xPos2 = 20;
        break;
        case 1:
        xPos1 = 248;
        xPos2 = 260;
        break;
        case 2:
        xPos1 = 368;
        xPos2 = 380;
        break;
        default:
        break;
      }
      drawTriangle(xPos1, 30, xPos1, 42, xPos2, 36, WHITE);
    }
    // drawing current state
    if (currentState >= 0) {
      switch (currentState) {
        case 0:
        xPos1 = 8;
        xPos2 = 20;
        break;
        case 1:
        xPos1 = 248;
        xPos2 = 260;
        break;
        case 2:
        xPos1 = 368;
        xPos2 = 380;
        break;
        default:
        break;
      }
      drawTriangle(xPos1, 30, xPos1, 42, xPos2, 36, DGRAY);
    }
  }

  void View::switchBetweenLayers(const Layer& layer_, const Beat& beat_, int8_t previousState) {
    drawInstA(layer_);
    drawInstB(layer_);
    if (layer_.getLastActiveBeat() != -1) {
      drawFill(beat_);
    } else if (fillNumberClean != -1) {
      cleanFill();
    }
  }

  void View::drawInstA(const Layer& layer_) {
    if (&layer_ != NULL) {
      uint8_t instA = layer_.getInstAMidi();
      drawMenuNumber(instAClean, kInstADigit, kInstAXPos, kDataYPos, WHITE);
      drawMenuNumber(instA, kInstADigit, kInstAXPos, kDataYPos, BLACK);
      instAClean = instA;
    }
  }

  void View::drawInstB(const Layer& layer_) {
    if (&layer_ != NULL) {
      uint8_t instB = layer_.getInstBMidi();
      drawMenuNumber(instBClean, kInstBDigit, kInstBXPos, kDataYPos, WHITE);
      drawMenuNumber(instB, kInstBDigit, kInstBXPos, kDataYPos, BLACK);
      instBClean = instB;
    }
  }

  void View::drawFill(const Beat& beat_) {
    if (&beat_ != NULL) {
      // checking if there is a new fill data
      if (beat_.getFill() != fillNumberClean) {
        // clearing old fill name
        fillRect(kFillNameXPos, kDataYPos, 57, 13, WHITE);
        // drawing fill name
        uint16_t fillNameXPos = kFillNameXPos;
        uint16_t fillNameYPos = kDataYPos;
        for(uint8_t i = 0; i < kFillNameDigit; i++) {
          drawMenuLetter(getFillName(beat_.getFill(), i), fillNameXPos, fillNameYPos, BLACK);
          fillNameXPos += kMenuDigitOffset;
        }
        // clearing old fill diagram
        fillRect(kFillGraphStartXPos, kDataYPos, kFillGraphLength, 12, WHITE);
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
          uint16_t fillDiagXPos = int(kFillGraphStartXPos + kFillGraphLength * double(quantaTimeRatio) / quantaTotalTimeRatio);
          float vLine = kFillGraphHeight * float(quantaVolume) / kMaxVolume;
          drawFastVLine(fillDiagXPos, 42 - vLine, vLine, BLACK);
        }
        // adjusting fillNumberClean
        fillNumberClean = beat_.getFill();
      }
    }
  }

  void View::cleanFill() {
    // clearing old fill name
    fillRect(kFillNameXPos, kDataYPos, 57, 13, WHITE);
    // clearing old fill diagram
    fillRect(kFillGraphStartXPos, kDataYPos, kFillGraphLength, kFillGraphHeight, WHITE);
    uint16_t fillNameXPos = kFillNameXPos;
    uint16_t fillNameYPos = kDataYPos;
    // drawing empty fill name (hypen)
    for(uint8_t i = 0; i < kFillNameDigit; i++) {
      drawMenuLetter('-', fillNameXPos, fillNameYPos, BLACK);
      fillNameXPos += kMenuDigitOffset;
    }
    fillNumberClean = -1;

  }

  // private info functions

  void View::drawMenuNumber(uint8_t num, uint8_t digit, uint16_t xPos, uint16_t yPos, uint16_t color) {
    uint8_t digitHundreds;
    uint8_t digitTens;
    uint8_t digitOnes;
    // drawing hundreds digit
    if ((digit >= kMinMenuDigit) && (digit <= kMaxMenuDigit)) {
      if (digit == 3) {
        digitHundreds = (num / 100);
        drawMenuDigit(digitHundreds, xPos, yPos, color);
        xPos += kMenuDigitOffset;
      }
    }
    // drawing tens digit
    if (digit >= 2) {
      digitTens = (num % 100) / 10;
      drawMenuDigit(digitTens, xPos, yPos, color);
      xPos += kMenuDigitOffset;
    }
    // drawing ones digit
    digitOnes = num % 10;
    drawMenuDigit(digitOnes, xPos, yPos, color);

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
