#include "Keyboard.h"

namespace Zebra {

  Keyboard::Keyboard()
  : rhythmSelectButton(kRhythmSelectButtonPin)
  , layer0SelectButton(kLayer0SelectButtonPin)
  , layer1SelectButton(kLayer1SelectButtonPin)
  , layer2SelectButton(kLayer2SelectButtonPin)
  , layer3SelectButton(kLayer3SelectButtonPin)

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
