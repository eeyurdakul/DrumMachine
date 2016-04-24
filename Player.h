#pragma once

#include "Arduino.h"
#include "Rhythm.h"
#include "View.h"

namespace Zebra {

  class Player {
  private:
    Rhythm& rhythmRef;
    View& viewRef;
    bool active;
    uint32_t time;
    uint32_t delay;
    uint32_t delayRegister;
  public:
    Player(Rhythm& rhythm_, View& view_);
    ~Player();
    void initialize();
    void reset();
    void play();
    void stop();
    void setActive(bool active_);
    bool getActive() const;
    void setTime(uint32_t time_);
    uint32_t getTime() const;
    void calculateDelay();
    uint32_t getDelay() const;
    void calculateDelayRegister();
    uint32_t getDelayRegister() const;
  };
}
