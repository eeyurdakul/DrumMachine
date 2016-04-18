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

}
