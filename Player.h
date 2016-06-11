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
    double period;
    double frequency;
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
    double getPeriod() const;
    void calculateFrequency();
    double getFrequency() const;
    void calculateAndStartTimer();
    Midi& getMidi();
  };
}
