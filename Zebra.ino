#include "Rhythm.h"
#include "View.h"
#include "Controller.h"

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
    if (rhythm.getPlayTime() < rhythm.getSongTime()) {
      rhythm.incrementPlayTime();
    } else {
      rhythm.restartPlayTime();
      view.restartPlayBar();
    }
  }
}
