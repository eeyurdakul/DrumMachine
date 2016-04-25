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
    uint32_t period;
    uint32_t frequency;
    uint32_t matchRegister;
  public:
    Player(Rhythm& rhythm_, View& view_);
    ~Player();
    void initialize();
    void reset();
    void play();
    void stop();
    void setActive(bool active_);
    bool getActive() const;
    void calculatePeriod();
    uint32_t getPeriod() const;
    void calculateFrequency();
    uint32_t getFrequency() const;
    void calculateMatchRegister();
    uint32_t getMatchRegister() const;
    void setTimerMatchRegister();
  };
}
