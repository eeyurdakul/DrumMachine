#include "Keyboard.h"

namespace Zebra {

  Keyboard::Keyboard()
  : rhythmSelectButton(kRhythmSelectButtonPin)
  , layer0SelectButton(kLayer0SelectButtonPin)
  , layer1SelectButton(kLayer1SelectButtonPin)
  , layer2SelectButton(kLayer2SelectButtonPin)
  , layer3SelectButton(kLayer3SelectButtonPin)

  , selectButton(kSelectButtonPin)
  , layer0ChannelButton(kLayer0ChannelButtonPin)
  , layer1ChannelButton(kLayer1ChannelButtonPin)
  , layer2ChannelButton(kLayer2ChannelButtonPin)
  , layer3ChannelButton(kLayer3ChannelButtonPin)

  , upButton(kUpButtonPin)
  , downButton(kDownButtonPin)
  , rightButton(kRightButtonPin)
  , leftButton(kLeftButtonPin)

  , recordButton(kRecordButtonPin)
  , playButton(kPlayButtonPin)
  , resetButton(kResetButtonPin)

  , beatAButton(kBeatAButtonPin)
  , beatBButton(kBeatBButtonPin)
  , beatClearButton(kBeatClearButtonPin) {}

  Keyboard::~Keyboard() {}
}
