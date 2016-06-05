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
    Button upButton;
    Button downButton;
    Button rightButton;
    Button leftButton;
    Button instAUpButton;
    Button instADownButton;
    Button instBUpButton;
    Button instBDownButton;
    Button recordButton;
    Button playButton;
    Button resetButton;
    Button beatAButton;
    Button beatBButton;
    Button beatClearButton;
  };
}
