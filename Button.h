#pragma once

#include <Arduino.h>
#include "Constant.h"

namespace Zebra {

  class Button {
  private:
    uint8_t pin;
    bool state;
    bool lastState;
    uint32_t lastDebounceTime;
  public:
    Button(uint8_t pin_);
    ~Button();
    void setState(bool state_);
    bool getState() const;
    void setLastState(bool lastState_);
    bool getLastState() const;
    bool checkStatus();
  };
}
