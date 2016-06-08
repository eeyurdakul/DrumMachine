#pragma once

#include "Button.h"
#include "Constant.h"

namespace Zebra {

  class Keyboard {
  private:
  public:
    Keyboard();
    ~Keyboard();
    Button rhythmMenuSelectButton;
    Button layer0SelectButton;
    Button layer1SelectButton;
    Button layer2SelectButton;
    Button layer3SelectButton;
    Button layer0ChannelButton;
    Button layer1ChannelButton;
    Button layer2ChannelButton;
    Button layer3ChannelButton;
    Button upButton;
    Button downButton;
    Button rightButton;
    Button leftButton;
    Button recordButton;
    Button playButton;
    Button resetButton;
    Button beatAButton;
    Button beatBButton;
    Button beatClearButton;
  };
}
