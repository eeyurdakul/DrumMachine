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
  // info rhythm variables
  , currentInfoRhythmLayout(0b10000000)
  , previousInfoRhythmLayout(0b00000000)
  , tempoClean(0)
  , metronomeClean(0)
  , barClean(0)
  , measureClean(0)
  , loadClean(0)
  , saveClean(0)
  , outputClean(0)
  , quantizeClean(0)
  // info layer variables
  , instAClean(0)
  , instBClean(0)
  , fillNumberClean(255)
  , switchInfoToRhythmFlag(false)
  , switchInfoToLayerFlag(false)
  , switchInfoBetweenLayersFlag(false)
  , switchLayer(0)
  , switchState(0) {}

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
    checkSwitchInfoToRhythm();
    checkSwitchInfoToLayer();
    checkSwitchInfoBetweenLayers();
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

  // rhythm functions

  void View::drawRhythmSelected() {
    // statement
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

  // info rhythm functions

  void View::drawTempo() {
    uint16_t background;
    uint16_t foreground;
    if (currentInfoRhythmLayout >> 7) {
      background = BLACK;
      foreground = WHITE;
    } else {
      background = WHITE;
      foreground = BLACK;
    }
    if (currentInfoRhythmLayout != previousInfoRhythmLayout) {
      fillRect(0, 0, 120, kInfoHeight, background);
      setTextColor(foreground);
      setCursor(8, 8);
      println(F("TEMPO"));
    }
    //drawInfoNumber(tempoClean, kTempoDigit, kTempoXPos, kTempoYPos, background);
    //drawInfoNumber(rhythmRef.getTempo(), kTempoDigit, kTempoXPos, kTempoYPos, foreground);
    //tempoClean = rhythmRef.getTempo();
  }

  void View::drawMetronome() {}

  void View::drawBar() {}

  void View::drawMeasure() {}

  void View::drawLoad() {}

  void View::drawSave() {}

  void View::drawOutput() {}

  void View::drawQuantize() {}

  //////////////////////

  void View::drawInfoRhythmAll() {
    fillNumberClean = -1;
    drawInfoRhythmTempo();
    drawInfoRhythmQuantize();
    drawInfoRhythmBar();
    drawInfoRhythmMeasure();
  }

  void View::drawInfoRhythmBase() {
    fillRect(0, 0, 480, kInfoHeight, WHITE);
    drawFastVLine(120, 0, 50, BLACK);
    drawFastVLine(240, 0, 50, BLACK);
    drawFastVLine(360, 0, 50, BLACK);
    uint8_t x;
    uint8_t y;
    fillRect(0, 0, 121, 50, BLACK);
    fillTriangle(110, 5, 110, 15, 100, 10, BLACK);
    //drawFastHLine(7, 20, 114, BLACK);
    setTextColor(LGRAY);
    setCursor(7, 7);
    println(F("TEMPO"));
    setTextColor(BLACK);
    setCursor(127, 7);
    println(F("METRONOME"));
    setCursor(247, 7);
    println(F("BAR"));
    setCursor(367, 7);
    println(F("MEASURE"));
    drawInfoNumber(120, kTempoDigit, kTempoXPos - 1, kTempoYPos - 1, LGRAY);
    drawInfoNumber(120, kTempoDigit, kTempoXPos - 1 + 120, kTempoYPos - 1, BLACK);
  }

  void View::drawInfoRhythmTempo() {
    drawInfoNumber(tempoClean, kTempoDigit, kTempoXPos, kTempoYPos, BLACK);
    drawInfoNumber(rhythmRef.getTempo(), kTempoDigit, kTempoXPos, kTempoYPos, WHITE);
    tempoClean = rhythmRef.getTempo();
  }

  void View::drawInfoRhythmQuantize() {
    drawInfoNumber(quantizeClean, kQuantizeDigit, kQuantizeXPos, kQuantizeYPos, WHITE);
    drawInfoNumber(pgm_read_word(&kQuantizeLibrary[rhythmRef.getQuantize()]), kQuantizeDigit, kQuantizeXPos, kQuantizeYPos, BLACK);
    quantizeClean = pgm_read_word(&kQuantizeLibrary[rhythmRef.getQuantize()]);
  }

  void View::drawInfoRhythmBar() {
    drawInfoNumber(barClean, kBarDigit, kBarXPos, kBarYPos, WHITE);
    drawInfoNumber(rhythmRef.getBar(), kBarDigit, kBarXPos, kBarYPos, BLACK);
    barClean = rhythmRef.getBar();
  }

  void View::drawInfoRhythmMeasure() {
    drawInfoNumber(measureClean, kMeasureDigit, kMeasureXPos, kMeasureYPos, WHITE);
    drawInfoNumber(rhythmRef.getMeasure(), kMeasureDigit, kMeasureXPos, kMeasureYPos, BLACK);
    measureClean = rhythmRef.getMeasure();
  }

  // info layer functions

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

  void View::drawInfoLayerInstA(const Layer& layer_) {
    if (&layer_ != NULL) {
      uint8_t instA = layer_.getInstAMidi();
      // checking if there is a new inst data
      if ((switchInfoToLayerFlag) || (instA != instAClean)) {
        drawInfoNumber(instAClean, kInstADigit, kInstAXPos, kInstAYPos, WHITE);
        drawInfoNumber(instA, kInstADigit, kInstAXPos, kInstAYPos, BLACK);
        instAClean = instA;
      }
    }
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

  // private info functions

  void View::drawInfoNumber(uint8_t num, uint8_t digit, uint16_t xPos, uint16_t yPos, uint16_t color) {
    uint8_t digitHundreds;
    uint8_t digitTens;
    uint8_t digitOnes;

    if ((digit >= kMinInfoDigit) && (digit <= kMaxInfoDigit)) {
      if (digit == 3) {
        digitHundreds = (num / 100);
        drawInfoDigit(digitHundreds, xPos, yPos, color);
        xPos += kInfoDigitOffset;
      }
      if (digit >= 2) {
        digitTens = (num % 100) / 10;
        drawInfoDigit(digitTens, xPos, yPos, color);
        xPos += kInfoDigitOffset;
      }
      digitOnes = num % 10;
      drawInfoDigit(digitOnes, xPos, yPos, color);
    }
  }

  void View::drawInfoLetter(char letter, uint16_t xPos, uint16_t yPos, uint16_t color) {
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
    drawInfoDigit(letterDigit, xPos, yPos, color);
  }

  void View::drawInfoDigit(uint8_t digit, uint16_t xPos, uint16_t yPos, uint16_t color) {
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

  // info switch functions

  void View::switchInfoToRhythm() {
    switchInfoToRhythmFlag = true;
    switchState = 0;
  }

  void View::switchInfoToLayer(const Layer& layer_) {
    if (&layer_ != NULL) {
      switchInfoToLayerFlag = true;
      switchLayer = layer_.getNumber();
      switchState = 0;
    }
  }

  void View::switchInfoBetweenLayers(const Layer& layer_) {
    if (&layer_ != NULL) {
      switchInfoBetweenLayersFlag = true;
      switchLayer = layer_.getNumber();
      switchState = 0;
    }
  }

  void View::checkSwitchInfoToRhythm() {
    if (switchInfoToRhythmFlag) {
      switch (switchState) {
        // clearing all
        case 0:
        fillRect(0, 0, 480, kInfoHeight, WHITE);
        switchState += 1;
        break;
        // drawing rhythm elements
        case 1:
        fillRect(0, 0, 121, kInfoHeight - 1, BLACK);
        drawFastVLine(120, 0, 50, BLACK);
        switchState += 1;
        break;
        case 2:
        drawFastVLine(240, 0, 50, BLACK);
        switchState += 1;
        break;
        case 3:
        drawFastVLine(360, 0, 50, BLACK);
        switchState += 1;
        break;
        case 4:
        setTextColor(WHITE);
        setCursor(8, 8);
        println(F("TEMPO"));
        switchState += 1;
        break;
        case 5:
        setTextColor(BLACK);
        setCursor(128, 8);
        println(F("QUANTIZE"));
        switchState += 1;
        break;
        case 6:
        setCursor(248, 8);
        println(F("BAR"));
        switchState += 1;
        break;
        case 7:
        setCursor(368, 8);
        println(F("MEASURE"));
        switchState += 1;
        break;
        case 8:
        drawInfoRhythmTempo();
        switchState += 1;
        break;
        case 9:
        drawInfoRhythmQuantize();
        switchState += 1;
        break;
        case 10:
        drawInfoRhythmBar();
        switchState += 1;
        break;
        case 11:
        drawInfoRhythmMeasure();
        switchState = 0;
        switchInfoToRhythmFlag = false;
        break;
        default:
        break;
      }
    }
  }

  void View::checkSwitchInfoToLayer() {
    if (switchInfoToLayerFlag) {
      switch (switchState) {
        // clearing all
        case 0:
        fillRect(0, 0, 480, kInfoHeight, WHITE);
        switchState += 1;
        break;
        // drawing layer elements
        case 1:
        drawFastVLine(240, 0, 20, BLACK);
        switchState += 1;
        break;
        case 2:
        drawFastVLine(360, 0, 20, BLACK);
        switchState += 1;
        break;
        case 3:
        drawFastHLine(90, 45, 120, BLACK);
        for(int i = 0; i < 12; i ++) {
          drawPixel(210, 9 + (3 * i), BLACK);
        }
        setTextColor(BLACK);
        switchState += 1;
        break;
        case 4:
        setCursor(8, 8);
        println(F("FILL"));
        switchState += 1;
        break;
        case 5:
        setCursor(248, 8);
        println(F("INST A"));
        switchState += 1;
        break;
        case 6:
        setCursor(368, 8);
        println(F("INST B"));
        switchState += 1;
        break;
        case 7:
        if (rhythmRef.getLayer(switchLayer).getLastActiveBeat() != -1) {
          drawInfoFill(rhythmRef.getLayer(switchLayer).getBeat(0));
        } else {
          cleanInfoFill();
        }
        switchState += 1;
        break;
        case 8:
        drawInfoLayerInstA(rhythmRef.getLayer(switchLayer));
        switchState += 1;
        break;
        case 9:
        drawInfoLayerInstB(rhythmRef.getLayer(switchLayer));
        switchState = 0;
        switchInfoToLayerFlag = false;
        break;
        default:
        break;
      }
    }
  }

  void View::checkSwitchInfoBetweenLayers() {
    if (switchInfoBetweenLayersFlag) {
      switch (switchState) {
        case 0:
        if (rhythmRef.getLayer(switchLayer).getLastActiveBeat() != -1) {
          drawInfoFill(rhythmRef.getLayer(switchLayer).getBeat(0));
        } else {
          cleanInfoFill();
        }
        switchState += 1;
        break;
        case 1:
        drawInfoLayerInstA(rhythmRef.getLayer(switchLayer));
        switchState += 1;
        break;
        case 2:
        drawInfoLayerInstB(rhythmRef.getLayer(switchLayer));
        switchState = 0;
        switchInfoBetweenLayersFlag = false;
        break;
        default:
        break;
      }
    }
  }

  // debug functions

  void View::debug(uint32_t var) {
    fillRect(40, 260, 200, 20, BLACK);
    setTextColor(WHITE);
    setCursor(50, 265);
    print(var);
  }
}
