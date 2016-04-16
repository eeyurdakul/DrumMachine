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
