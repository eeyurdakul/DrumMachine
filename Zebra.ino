#include "Rhythm.h"
#include "View.h"
#include "Controller.h"

uint16_t playXRatio = 0;

using namespace Zebra;

Rhythm rhythm;
View view(rhythm);
Player player(rhythm, view);
Controller controller(rhythm, view, player);

void setup() {
  Serial.begin(9600);
  view.initialize();
  player.initialize();
  controller.initialize();
}

void loop() {
  controller.checkKeyboardStatus();
  view.drawPlayBar();
}

ISR(TIMER1_COMPA_vect) {
  if (player.getActive()) {
    rhythm.incrementPlayTime();
  }
}
