#include "Rhythm.h"
#include "View.h"
#include "Controller.h"

using namespace Zebra;

Rhythm rhythm;
View view(rhythm);
Controller controller(rhythm, view);

void setup() {
  Serial.begin(9600);
  view.initialize();
  controller.initialize();
}

void loop() {
  controller.checkKeyboardStatus();
}
