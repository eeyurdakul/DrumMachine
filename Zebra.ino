#include "Rhythm.h"
#include "Timeframe.h"
#include "Beat.h"

using namespace Zebra;

Timeframe test;
Beat beat;
Rhythm rhythm;

void setup() {
  Serial.begin(9600);
  beat.setActive(1);
  beat.setInst(1);
  beat.setVolume(2);
  beat.setTimeframe(450);
  beat.setSector(1);
  Serial.println(beat.getSector());
}

void loop() {
  //statements
}
