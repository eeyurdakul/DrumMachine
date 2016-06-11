#include "Button.h"

namespace Zebra {

  Button::Button(uint8_t pin_)
  : pin(pin_)
  , state(false)
  , lastState(false) {
    pinMode(pin, INPUT);
  }

  Button::~Button() {}

  void Button::setState(bool state_) {
    state = state_;
  }

  bool Button::getState() const {
    return state;
  }

  void Button::setLastState(bool lastState_) {
    lastState = lastState_;
  }

  bool Button::getLastState() const {
    return lastState;
  }

  bool Button::checkStatus() {
    bool active = false;
    bool reading = digitalRead(pin);
    if (reading != lastState) {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > kDebounceDelay) {
      if (reading != state) {
        state = reading;
        if (state == true) {
          active = true;
        }
      }
    }
    lastState = reading;
    return active;
  }
}
