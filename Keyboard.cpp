#include "Keyboard.h"

namespace Zebra {

  Keyboard::Keyboard()
  : menuSelectButton(kMenuSelectButtonPin)
  , layer0SelectButton(kLayer0SelectButtonPin)
  , layer1SelectButton(kLayer1SelectButtonPin)
  , layer2SelectButton(kLayer2SelectButtonPin)
  , layer3SelectButton(kLayer3SelectButtonPin)

  , layer0ChannelButton(kLayer0ChannelButtonPin)
  , layer1ChannelButton(kLayer1ChannelButtonPin)
  , layer2ChannelButton(kLayer2ChannelButtonPin)
  , layer3ChannelButton(kLayer3ChannelButtonPin)

  , upButton(kUpButtonPin)
  , downButton(kDownButtonPin)
  , rightButton(kRightButtonPin)
  , leftButton(kLeftButtonPin)

  , instAUpButton(kInstAUpButtonPin)
  , instADownButton(kInstADownButtonPin)
  , instBUpButton(kInstBUpButtonPin)
  , instBDownButton(kInstBDownButtonPin)

  , recordButton(kRecordButtonPin)
  , playButton(kPlayButtonPin)
  , resetButton(kResetButtonPin)

  , beatAButton(kBeatAButtonPin)
  , beatBButton(kBeatBButtonPin)
  , beatClearButton(kBeatClearButtonPin) {}

  Keyboard::~Keyboard() {}
}
