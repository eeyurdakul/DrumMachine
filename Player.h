#pragma once

#include "Arduino.h"
#include "Rhythm.h"
#include "View.h"
#include "Midi.h"

namespace Zebra {

  class Player {
  private:
    Rhythm& rhythmRef;
    View& viewRef;
    Midi midi;
    bool playActive;
    bool recordActive;
    uint32_t period;
    uint32_t frequency;
    uint32_t matchRegister;
  public:
    Player(Rhythm& rhythm_, View& view_);
    ~Player();
    void initialize();
    void reset();
    void play();
    void record();
    void stop();
    void setPlayActive(bool active_);
    bool getPlayActive() const;
    void setRecordActive(bool active_);
    bool getRecordActive() const;
    void calculatePeriod();
    uint32_t getPeriod() const;
    void calculateFrequency();
    uint32_t getFrequency() const;
    void calculateMatchRegister();
    uint32_t getMatchRegister() const;
    void setTimerMatchRegister();
    Midi& getMidi();
  };
}
