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

  // Keyboard.h

  const uint8_t kRhythmSelectButtonPin = 24;
  const uint8_t kLayer0SelectButtonPin = 25;
  const uint8_t kLayer1SelectButtonPin = 26;
  const uint8_t kLayer2SelectButtonPin = 27;
  const uint8_t kLayer3SelectButtonPin = 28;

  const uint8_t kRhythmTempoUpButtonPin = 29;
  const uint8_t kRhythmTempoDownButtonPin = 30;
  const uint8_t kRhythmQuantizeUpButtonPin = 31;
  const uint8_t kRhythmQuantizeDownButtonPin = 32;

  const uint8_t kRhythmBarUpButtonPin = 42;
  const uint8_t kRhythmBarDownButtonPin = 43;
  const uint8_t kRhythmMeasureUpButtonPin = 44;
  const uint8_t kRhythmMeasureDownButtonPin = 45;

  const uint8_t kRecordButtonPin = 46;
  const uint8_t kPlayButtonPin = 47;
  const uint8_t kResetButtonPin = 48;

  const uint8_t kBeatAButtonPin = 49;
  const uint8_t kBeatBButtonPin = 50;
  const uint8_t kBeatClearButtonPin = 51;

  const uint8_t kMetronomeButtonPin = 4;

  const uint8_t kRecordLedPin = 52;
  const uint8_t kPlayLedPin = 53;

  // View.h

  // play constants
  const uint16_t kPlayY = 56;
  const uint16_t kPlayColor0 = WHITE;
  const uint16_t kPlayColor1 = BLACK;
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
  const uint16_t kFillNameXPos = 10;
  const uint16_t kFillNameYPos = 32;
  const uint8_t kFillNameLetterCount = 5;
  const uint16_t kInstAXPos = 250;
  const uint16_t kInstAYPos = 32;
  const uint8_t kInstADigit = 2;
  const uint16_t kInstBXPos = 370;
  const uint16_t kInstBYPos = 32;
  const uint8_t kInstBDigit = 2;

  // Rhythm.h

  const uint8_t kMinRhythmTempo = 60;
  const uint8_t kMaxRhythmTempo = 160;
  const uint8_t kMinRhythmQuantize = 0;
  const uint8_t kMaxRhythmQuantize = 6;
  const uint8_t kMinRhythmBar = 1;
  const uint8_t kMaxRhythmBar = 8;
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

  const uint16_t kTimeframeLibrarySize = 2048;
  const uint16_t kMaxTime = 4095;
  const uint8_t kMaxVolume = 3;   // 0 -> passive, 1 -> 30, 2 -> 60, 3 -> 90

  // Button.h

  const uint8_t kDebounceDelay = 50;

  // Player.h

  const uint16_t kTimerPreScaler = 1024;
  const uint32_t kMicroSecondsinOneSecond = 1000000;
  const uint32_t kMicroSecondsinOneMinute = 60000000;

  // Midi.h

  const uint16_t kMidiBaudRate = 31250;
  const uint8_t kMidiInitialChannel = 9;
  const uint8_t kMidiVolume0 = 0;
  const uint8_t kMidiVolume1 = 40;
  const uint8_t kMidiVolume2 = 70;
  const uint8_t kMidiVolume3 = 100;
}
