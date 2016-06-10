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

const uint8_t kRhythmMenuSelectButtonPin = 24;
const uint8_t kLayer0SelectButtonPin = 25;
const uint8_t kLayer1SelectButtonPin = 26;
const uint8_t kLayer2SelectButtonPin = 27;
const uint8_t kLayer3SelectButtonPin = 28;

const uint8_t kLayer0ChannelButtonPin = 42;
const uint8_t kLayer1ChannelButtonPin = 43;
const uint8_t kLayer2ChannelButtonPin = 44;
const uint8_t kLayer3ChannelButtonPin = 45;

const uint8_t kUpButtonPin = 29;
const uint8_t kDownButtonPin = 30;
const uint8_t kRightButtonPin = 31;
const uint8_t kLeftButtonPin = 32;

const uint8_t kRecordButtonPin = 46;
const uint8_t kPlayButtonPin = 47;
const uint8_t kResetButtonPin = 48;

const uint8_t kBeatAButtonPin = 49;
const uint8_t kBeatBButtonPin = 50;
const uint8_t kBeatClearButtonPin = 51;

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
// rhythm menu constants
const uint8_t kMaxRhythmMenu = 7;
const uint8_t kTempoBox = 0;
const uint8_t kMetronomeBox = 1;
const uint8_t kBarBox = 2;
const uint8_t kMeasureBox = 3;
const uint8_t kLoadBox = 4;
const uint8_t kSaveBox = 5;
const uint8_t kOutputBox = 6;
const uint8_t kQuantizeBox = 7;
const uint16_t kRhythmMenuBoxXPos[] = {0, 120, 240, 360, 0, 120, 240, 360};
const uint8_t kRhythmMenuBoxYPos = 0;
const uint8_t kRhythmMenuBoxWidth = 119;
const uint8_t kRhythmMenuBoxHeight = 50;
const uint16_t kRhythmMenuHeaderXPos[] = {7, 127, 247, 367, 7, 127, 247, 367};
const uint8_t kRhythmMenuHeaderYPos = 7;
const uint8_t kRhythmMenuDataDigit[] = {3, 2, 2, 2, 2, 2, 2, 2};
const uint16_t kRhythmMenuDataXPos[] = {7, 127, 247, 367, 7, 127, 247, 367};
const uint8_t kRhythmMenuDataYPos = 31;
// layer menu constants
const uint8_t kMaxLayerMenu = 2;
const uint8_t kFillBox = 0;
const uint8_t kInstABox = 1;
const uint8_t kInstBBox = 2;
const uint16_t kLayerMenuBoxXPos[] = {0, 240, 360};
const uint8_t kLayerMenuBoxYPos = 0;
const uint8_t kLayerMenuBoxWidth = 119;
const uint8_t kLayerMenuBoxWidth2 = 239;
const uint8_t kLayerMenuBoxHeight = 50;
const uint16_t kLayerMenuHeaderXPos[] = {7, 247, 367};
const uint8_t kLayerMenuHeaderYPos = 7;
const uint8_t kLayerMenuDataDigit[] = {5, 2, 2};
const uint16_t kLayerMenuDataXPos[] = {7, 247, 367};
const uint8_t kLayerMenuDataYPos = 31;
// menu digit constants
const uint8_t kMinMenuDigit = 0;
const uint8_t kMaxMenuDigit = 3;
const uint8_t kMenuDigitOffset = 12;

// Rhythm.h

const uint8_t kMinTempo = 60;
const uint8_t kMaxTempo = 160;
const uint8_t kMinBar = 1;
const uint8_t kMaxBar = 8;
const uint8_t kMinMeasure = 1;
const uint8_t kMaxMeasure = 8;
const uint8_t kMinLoad = 1;
const uint8_t kMaxLoad = 32;
const uint8_t kMinSave = 1;
const uint8_t kMaxSave = 32;
const uint8_t kMinQuantize = 0;
const uint8_t kMaxQuantize = 6;
const uint8_t kInitialTempo = 120;
const bool kInitialMetronome = 1;   // 0 --> off, 1 --> on
const uint8_t kInitialBar = 4;
const uint8_t kInitialMeasure = 4;
const uint8_t kInitialLoad = 1;
const uint8_t kInitialSave = 1;
const bool kInitialOutput = 0;      // 0 --> audio, 1 --> midi
const uint8_t kInitialQuantize = 0;
const uint8_t kLayerLibrarySize = 4;
const uint16_t kMeasureTime = 64;
const uint8_t kQuantizeLibrary[7] PROGMEM = {0, 1, 2, 4, 8, 16, 32};

// Layer.h

const uint8_t kMinLayerVolume = 0;
const uint8_t kMaxLayerVolume = 10;
const uint8_t kMinLayerInstMidi = 0;
const uint8_t kMaxLayerInstMidi = 127;
const uint8_t kMinLayerInstAudio = 0;
const uint8_t kMaxLayerInstAudio = 127;
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
