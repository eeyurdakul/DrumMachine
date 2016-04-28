#pragma once

#include "Button.h"
#include "Constant.h"

namespace Zebra {

  class Keyboard {
  private:
  public:
    Keyboard();
    ~Keyboard();
    Button rhythmSelectButton;
    Button layer0SelectButton;
    Button layer1SelectButton;
    Button layer2SelectButton;
    Button layer3SelectButton;
    Button rhythmBarUpButton;
    Button rhythmBarDownButton;
    Button rhythmMeasureUpButton;
    Button rhythmMeasureDownButton;
    Button recordButton;
    Button playStopButton;
    Button resetButton;
    Button metronomeButton;
    Button beatAButton;
    Button beatBButton;
    Button beatCButton;
    Button beatDButton;
  };

}
