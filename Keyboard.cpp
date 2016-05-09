#include "Keyboard.h"

namespace Zebra {

  Keyboard::Keyboard()
  : rhythmSelectButton(kRhythmSelectButtonPin)
  , layer0SelectButton(kLayer0SelectButtonPin)
  , layer1SelectButton(kLayer1SelectButtonPin)
  , layer2SelectButton(kLayer2SelectButtonPin)
  , layer3SelectButton(kLayer3SelectButtonPin)

  , rhythmTempoUpButton(kRhythmTempoUpButtonPin)
  , rhythmTempoDownButton(kRhythmTempoDownButtonPin)
  , rhythmQuantizeUpButton(kRhythmQuantizeUpButtonPin)
  , rhythmQuantizeDownButton(kRhythmQuantizeDownButtonPin)

  , rhythmBarUpButton(kRhythmBarUpButtonPin)
  , rhythmBarDownButton(kRhythmBarDownButtonPin)
  , rhythmMeasureUpButton(kRhythmMeasureUpButtonPin)
  , rhythmMeasureDownButton(kRhythmMeasureDownButtonPin)

  , recordButton(kRecordButtonPin)
  , playButton(kPlayButtonPin)
  , resetButton(kResetButtonPin)
  
  , beatAButton(kBeatAButtonPin)
  , beatBButton(kBeatBButtonPin)
  , beatClearButton(kBeatClearButtonPin)

  , metronomeButton(kMetronomeButtonPin) {}

  Keyboard::~Keyboard() {}
}
