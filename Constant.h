#pragma once

namespace Zebra {

  enum Color {
    BLACK   = 0x0000,
    BLUE    = 0x001F,
    RED     = 0xF800,
    GREEN   = 0x07E0,
    CYAN    = 0x07FF,
    MAGENTA = 0xF81F,
    YELLOW  = 0xFFE0,
    WHITE   = 0xFFFF,
    LGRAY   = 0x9CF3,
    MGRAY   = 0x8C51,
    DGRAY   = 0x31A6
  };

  // View.h
  // play constants
  const uint16_t kPlayY = 60;
  const uint16_t kPlayColor0 = WHITE;
  const uint16_t kPlayColor1 = MGRAY;
  // layer constants
  const uint16_t kSongStartX = 48;
  const uint16_t kSongEndX = 408;
  const uint16_t kSongX = 360;
  const uint16_t kStartY = 0;
  // info constants
  const uint8_t kInfoHeight = 52;
  const uint8_t kMinInfoDigit = 0;
  const uint8_t kMaxInfoDigit = 3;
  const uint8_t kInfoDigitOffset = 12;
  // info rhythm constants
  const uint16_t kTempoXPos = 10;
  const uint16_t kTempoYPos = 32;
  const uint8_t kTempoDigit = 3;
  const uint16_t kQuantizeXPos = 130;
  const uint16_t kQuantizeYPos = 32;
  const uint8_t kQuantizeDigit = 2;
  const uint16_t kBarXPos = 250;
  const uint16_t kBarYPos = 32;
  const uint8_t kBarDigit = 2;
  const uint16_t kMeasureXPos = 370;
  const uint16_t kMeasureYPos = 32;
  const uint8_t kMeasureDigit = 2;
  // info layer constants
  const uint16_t kInstAXPos = 10;
  const uint16_t kInstAYPos = 32;
  const uint8_t kInstADigit = 2;
  const uint16_t kInstBXPos = 130;
  const uint16_t kInstBYPos = 32;
  const uint8_t kInstBDigit = 2;
  const uint16_t kFillNameXPos = 250;
  const uint16_t kFillNameYPos = 32;
  const uint8_t kFillNameLetterCount = 5;

  // Rhythm.h
  const uint8_t kMinRhythmTempo = 60;
  const uint8_t kMaxRhythmTempo = 160;
  const uint8_t kMinRhythmQuantize = 0;
  const uint8_t kMaxRhythmQuantize = 6;
  const uint8_t kMinRhythmBar = 1;
  const uint8_t kMaxRhythmBar = 4;
  const uint8_t kMinRhythmMeasure = 1;
  const uint8_t kMaxRhythmMeasure = 8;
  const uint8_t kInitialTempo = 120;
  const uint8_t kInitialQuantize = 0;
  const uint8_t kInitialBar = 4;
  const uint8_t kInitialMeasure = 4;
  const uint8_t kLayerLibrarySize = 4;
  const uint16_t kMeasureTime = 64;
  const uint8_t kQuantizeLibrary[7] PROGMEM = {0, 1, 2, 4, 8, 16, 32};

  // Layer.h
  const uint8_t kMinLayerVolume = 0;
  const uint8_t kMaxLayerVolume = 10;
  const uint8_t kMinLayerInst = 0;
  const uint8_t kMaxLayerInst = 127;
  const uint8_t kBeatLibrarySize = 32;

  // Fill.h
  const uint8_t kFillLibrarySize = 4;

  // Timeline.h
  const uint16_t kTimeframeLibrarySize = 1024;
  const uint16_t kMaxTime = 2047;
  const uint8_t kMaxVolume = 3;   // 0 -> passive, 1 -> 30, 2 -> 60, 3 -> 90

  // Button.h
  const uint8_t kDebounceDelay = 50;

  // Player.h
  const uint16_t kTimerPreScaler = 64;
  const uint32_t kMicroSecondsinOneMinute = 60000000;
}
