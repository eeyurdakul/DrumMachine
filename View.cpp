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
  , rhythmTempoClean(0)
  , rhythmQuantizeClean(0)
  , rhythmBarClean(0)
  , rhythmMeasureClean(0)
  , layerInstAClean(0)
  , layerInstBClean(0)
  , fillNumberClean(255)
  , switchInfoFromLayerToRhythmFlag(false)
  , switchInfoFromRhythmToLayerFlag(false)
  , switchInfoFromLayerToLayerFlag(false)
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
    checkSwitchInfoFromLayerToRhythm();
    checkSwitchInfoFromRhythmToLayer();
    checkSwitchInfoFromLayerToLayer();
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

  // rhythm functions

  void View::drawRhythmSelectActive() {
    // statement
  }

  void View::drawSelectedFill(uint8_t selectedLayer, uint8_t selectedBeat) {
    fillRect(selectedFillXStartClean, selectedFillYClean, selectedFillXEndClean - selectedFillXStartClean + 1, 3, BLACK);
    // checking if layer is active and has at least one beat
    if ((selectedLayer < kLayerLibrarySize) && (rhythmRef.getLayer(selectedLayer).getLastActiveBeat() >= 0)) {
      // calculating starting beat time
      uint32_t beatTime = rhythmRef.getLayer(selectedLayer).getBeat(selectedBeat).getTime();
      uint32_t nextBeatTime;
      // calculating ending beat time
      if (selectedBeat == kBeatLibrarySize - 1) {
        nextBeatTime = rhythmRef.getSongTime();
      } else if (!rhythmRef.getLayer(selectedLayer).getBeat(selectedBeat + 1).getActive()) {
        nextBeatTime = rhythmRef.getSongTime();
      } else {
        nextBeatTime = rhythmRef.getLayer(selectedLayer).getBeat(selectedBeat + 1).getTime();
      }
      // calculating starting X position
      uint16_t xStartPos = kSongStartX + (beatTime * kSongX / rhythmRef.getSongTime());
      // calculating ending X position
      uint16_t xEndPos = kSongStartX +  (nextBeatTime * kSongX / rhythmRef.getSongTime());
      // calculating Y position
      uint16_t yPos = rhythmRef.getLayer(selectedLayer).getStartY() + 11;
      // drawing selected fill
      fillRect(xStartPos, yPos, xEndPos - xStartPos + 1, 3, rhythmRef.getLayer(selectedLayer).getColor());
      // saving X - Y positions to clean state
      selectedFillXStartClean = xStartPos;
      selectedFillXEndClean = xEndPos;
      selectedFillYClean = yPos;
    }
  }

  // layer functions

  void View::drawLayerAll(Layer& layer_) {
    drawLayerBase(layer_);
    drawLayerBeatActive(layer_);
    drawLayerFillActive(layer_);
    drawLayerSelectActive(layer_);
    drawLayerMeasure(layer_);
    drawLayerSong(layer_);
  }

  void View::drawLayerBase(const Layer& layer_) {
    const uint16_t kLayerBaseYPos = 66;
    drawFastHLine(10, layer_.getStartY() + kLayerBaseYPos, 460, layer_.getColor());
    fillRect(15, layer_.getStartY() + 29, 7, 7, layer_.getColor());
  }

  void View::drawLayerSelectActive(const Layer& layer_) {
    uint16_t color;
    if (layer_.getSelectActive()) {
      color = layer_.getColor();
    } else {
      color = BLACK;
    }
    fillRect(0, layer_.getStartY() + 29, 15, 7, color);
  }

  void View::drawLayerBeatActive(const Layer& layer_) {
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

  void View::drawLayerFillActive(const Layer& layer_) {
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

  void View::drawLayerMeasure(const Layer& layer_) {
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

  void View::drawLayerSong(Layer& layer_) {
    fillRect(kSongStartX, layer_.getStartY() + 23, kSongX + 1, 13, BLACK);
    for (uint8_t i = 0; i < kBeatLibrarySize; i++) {
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

  void View::drawLayerBeat(Layer& layer_, uint32_t time_, bool inst_) {
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

  void View::drawAllLayerMeasureAndSong() {
    for (uint8_t i = 0; i < kLayerLibrarySize; i++) {
      drawLayerMeasure(rhythmRef.getLayer(i));
      drawLayerSong(rhythmRef.getLayer(i));
    }
  }

  // info rhythm functions

  void View::drawInfoRhythmAll() {
    fillNumberClean = -1;
    drawInfoRhythmTempo();
    drawInfoRhythmQuantize();
    drawInfoRhythmBar();
    drawInfoRhythmMeasure();
  }

  void View::drawInfoRhythmBase() {
    fillRect(0, 0, 480, kInfoHeight, WHITE);
    drawFastVLine(120, 0, 20, BLACK);
    drawFastVLine(240, 0, 20, BLACK);
    drawFastVLine(360, 0, 20, BLACK);
    setTextColor(BLACK);
    setCursor(8, 8);
    println(F("TEMPO"));
    setCursor(128, 8);
    println(F("QUANTIZE"));
    setCursor(248, 8);
    println(F("BAR"));
    setCursor(368, 8);
    println(F("MEASURE"));
  }

  void View::drawInfoRhythmTempo() {
    drawInfoNumber(rhythmTempoClean, kTempoDigit, kTempoXPos, kTempoYPos, WHITE);
    drawInfoNumber(rhythmRef.getTempo(), kTempoDigit, kTempoXPos, kTempoYPos, BLACK);
    rhythmTempoClean = rhythmRef.getTempo();
  }

  void View::drawInfoRhythmQuantize() {
    drawInfoNumber(rhythmQuantizeClean, kQuantizeDigit, kQuantizeXPos, kQuantizeYPos, WHITE);
    drawInfoNumber(pgm_read_word(&kQuantizeLibrary[rhythmRef.getQuantize()]), kQuantizeDigit, kQuantizeXPos, kQuantizeYPos, BLACK);
    rhythmQuantizeClean = pgm_read_word(&kQuantizeLibrary[rhythmRef.getQuantize()]);
  }

  void View::drawInfoRhythmBar() {
    drawInfoNumber(rhythmBarClean, kBarDigit, kBarXPos, kBarYPos, WHITE);
    drawInfoNumber(rhythmRef.getBar(), kBarDigit, kBarXPos, kBarYPos, BLACK);
    rhythmBarClean = rhythmRef.getBar();
  }

  void View::drawInfoRhythmMeasure() {
    drawInfoNumber(rhythmMeasureClean, kMeasureDigit, kMeasureXPos, kMeasureYPos, WHITE);
    drawInfoNumber(rhythmRef.getMeasure(), kMeasureDigit, kMeasureXPos, kMeasureYPos, BLACK);
    rhythmMeasureClean = rhythmRef.getMeasure();
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
    drawFastVLine(120, 0, 20, BLACK);
    drawFastVLine(240, 0, 20, BLACK);
    // drawing fill graph base
    //drawFastVLine(330, 9, 36, BLACK);
    drawFastHLine(330, 45, 140, BLACK);
    for(int i = 0; i < 12; i ++) {
      drawPixel(470, 9 + (3 * i), BLACK);
    }
    setTextColor(BLACK);
    setCursor(8, 8);
    println(F("INST A"));
    setCursor(128, 8);
    println(F("INST B"));
    setCursor(248, 8);
    println(F("FILL"));
  }

  void View::drawInfoLayerInstA(const Layer& layer_) {
    drawInfoNumber(layerInstAClean, kInstADigit, kInstAXPos, kInstAYPos, WHITE);
    drawInfoNumber(layer_.getInstA(), kInstADigit, kInstAXPos, kInstAYPos, BLACK);
    layerInstAClean = layer_.getInstA();
  }

  void View::drawInfoLayerInstB(const Layer& layer_) {
    drawInfoNumber(layerInstBClean, kInstBDigit, kInstBXPos, kInstBYPos, WHITE);
    drawInfoNumber(layer_.getInstB(), kInstBDigit, kInstBXPos, kInstBYPos, BLACK);
    layerInstBClean = layer_.getInstB();
  }

  void View::drawInfoFill(const Beat& beat_) {
    // checking if there is a new fill or not
    if (beat_.getFill() != fillNumberClean) {
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
      fillRect(330, 9, 140, 36, WHITE);
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
        uint16_t fillDiagXPos = int(330 + 140 * double(quantaTimeRatio) / quantaTotalTimeRatio);
        float vLine = 36 * float(quantaVolume) / kMaxVolume;
        drawFastVLine(fillDiagXPos, 45 - vLine, vLine, BLACK);
      }
      // adjusting fillNumberClean
      fillNumberClean = beat_.getFill();
    }
  }

  void View::cleanInfoFill() {
    // clearing old fill name
    fillRect(kFillNameXPos, kFillNameYPos, 57, 13, WHITE);
    // clearing old fill diagram
    fillRect(330, 9, 140, 36, WHITE);
    fillNumberClean = -1;
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

  void View::switchInfoFromLayerToRhythm() {
    switchInfoFromLayerToRhythmFlag = true;
    switchState = 0;
  }

  void View::switchInfoFromRhythmToLayer(uint8_t layerNum) {
    switchInfoFromRhythmToLayerFlag = true;
    switchLayer = layerNum;
    switchState = 0;
  }

  void View::switchInfoFromLayerToLayer(uint8_t layerNum) {
    switchInfoFromLayerToLayerFlag = true;
    switchLayer = layerNum;
    switchState = 0;
  }

  void View::checkSwitchInfoFromLayerToRhythm() {
    if (switchInfoFromLayerToRhythmFlag) {
      switch (switchState) {
        // clearing layer elements
        case 0:
        setTextColor(WHITE);
        setCursor(8, 8);
        println(F("INST A"));
        switchState += 1;
        break;
        case 1:
        setCursor(128, 8);
        println(F("INST B"));
        switchState += 1;
        break;
        case 2:
        setCursor(248, 8);
        println(F("FILL"));
        switchState += 1;
        break;
        case 3:
        drawInfoNumber(layerInstAClean, kInstADigit, kInstAXPos, kInstAYPos, WHITE);
        switchState += 1;
        break;
        case 4:
        drawInfoNumber(layerInstBClean, kInstBDigit, kInstBXPos, kInstBYPos, WHITE);
        switchState += 1;
        break;
        case 5:
        fillRect(kFillNameXPos, kFillNameYPos, 57, 13, WHITE);
        switchState += 1;
        break;
        case 6:
        fillRect(330, 9, 141, 37, WHITE);
        switchState += 1;
        break;
        // drawing rhythm elements
        case 7:
        drawFastVLine(360, 0, 20, BLACK);
        setTextColor(BLACK);
        switchState += 1;
        break;
        case 8:
        setCursor(8, 8);
        println(F("TEMPO"));
        switchState += 1;
        break;
        case 9:
        setCursor(128, 8);
        println(F("QUANTIZE"));
        switchState += 1;
        break;
        case 10:
        setCursor(248, 8);
        println(F("BAR"));
        switchState += 1;
        break;
        case 11:
        setCursor(368, 8);
        println(F("MEASURE"));
        switchState += 1;
        break;
        case 12:
        drawInfoRhythmTempo();
        switchState += 1;
        break;
        case 13:
        drawInfoRhythmQuantize();
        switchState += 1;
        break;
        case 14:
        drawInfoRhythmBar();
        switchState += 1;
        break;
        case 15:
        drawInfoRhythmMeasure();
        switchState = 0;
        switchInfoFromLayerToRhythmFlag = false;
        break;
        default:
        break;
      }
    }
  }

  void View::checkSwitchInfoFromRhythmToLayer() {
    if (switchInfoFromRhythmToLayerFlag) {
      switch (switchState) {
        // clearing layer elements
        case 0:
        drawFastVLine(360, 0, 20, WHITE);
        setTextColor(WHITE);
        switchState += 1;
        break;
        case 1:
        setCursor(8, 8);
        println(F("TEMPO"));
        switchState += 1;
        break;
        case 2:
        setCursor(128, 8);
        println(F("QUANTIZE"));
        switchState += 1;
        break;
        case 3:
        setCursor(248, 8);
        println(F("BAR"));
        switchState += 1;
        break;
        case 4:
        setCursor(368, 8);
        println(F("MEASURE"));
        switchState += 1;
        break;
        case 5:
        drawInfoNumber(rhythmTempoClean, kTempoDigit, kTempoXPos, kTempoYPos, WHITE);
        switchState += 1;
        break;
        case 6:
        drawInfoNumber(rhythmQuantizeClean, kQuantizeDigit, kQuantizeXPos, kQuantizeYPos, WHITE);
        switchState += 1;
        break;
        case 7:
        drawInfoNumber(rhythmBarClean, kBarDigit, kBarXPos, kBarYPos, WHITE);
        switchState += 1;
        break;
        case 8:
        drawInfoNumber(rhythmMeasureClean, kMeasureDigit, kMeasureXPos, kMeasureYPos, WHITE);
        switchState += 1;
        break;
        // drawing layer elements
        case 9:
        drawFastHLine(330, 45, 140, BLACK);
        for(int i = 0; i < 12; i ++) {
          drawPixel(470, 9 + (3 * i), BLACK);
        }
        setTextColor(BLACK);
        switchState += 1;
        break;
        case 10:
        setCursor(8, 8);
        println(F("INST A"));
        switchState += 1;
        break;
        case 11:
        setCursor(128, 8);
        println(F("INST B"));
        switchState += 1;
        break;
        case 12:
        setCursor(248, 8);
        println(F("FILL"));
        switchState += 1;
        break;
        case 13:
        drawInfoLayerInstA(rhythmRef.getLayer(switchLayer));
        switchState += 1;
        break;
        case 14:
        drawInfoLayerInstB(rhythmRef.getLayer(switchLayer));
        switchState += 1;
        break;
        case 15:
        if (rhythmRef.getLayer(switchLayer).getLastActiveBeat() != -1) {
          drawInfoFill(rhythmRef.getLayer(switchLayer).getBeat(0));
        } else {
          cleanInfoFill();
        }
        switchState = 0;
        switchInfoFromRhythmToLayerFlag = false;
        break;
        default:
        break;
      }
    }
  }

  void View::checkSwitchInfoFromLayerToLayer() {
    if (switchInfoFromLayerToLayerFlag) {
      switch (switchState) {
        case 0:
        drawInfoLayerInstA(rhythmRef.getLayer(switchLayer));
        switchState += 1;
        break;
        case 1:
        drawInfoLayerInstB(rhythmRef.getLayer(switchLayer));
        switchState += 1;
        break;
        case 2:
        if (rhythmRef.getLayer(switchLayer).getLastActiveBeat() != -1) {
          drawInfoFill(rhythmRef.getLayer(switchLayer).getBeat(0));
        } else {
          cleanInfoFill();
        }
        switchState = 0;
        switchInfoFromLayerToLayerFlag = false;
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
