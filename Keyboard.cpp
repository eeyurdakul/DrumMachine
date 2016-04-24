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
  , playStopButton(39)
  , resetButton(40) {}

  Keyboard::~Keyboard() {}
}
