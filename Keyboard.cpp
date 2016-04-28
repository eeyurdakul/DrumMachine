#include "Keyboard.h"

namespace Zebra {

  Keyboard::Keyboard()
  : rhythmSelectButton(30)
  , layer0SelectButton(31)
  , layer1SelectButton(32)
  , layer2SelectButton(33)
  , layer3SelectButton(34)
  , rhythmBarUpButton(35)
  , rhythmBarDownButton(36)
  , rhythmMeasureUpButton(37)
  , rhythmMeasureDownButton(38)
  , recordButton(39)
  , playStopButton(40)
  , resetButton(41)
  , metronomeButton(42)
  , beatAButton(43)
  , beatBButton(44)
  , beatCButton(45)
  , beatDButton(46) {}

  Keyboard::~Keyboard() {}
}
