#include "Layer.h"

using namespace Zebra;

Layer layer(0);

void setup() {
  Serial.begin(9600);
  layer.setBeat(500, 2, 1);
  Serial.println(layer.getBeat(0).getTime());
  Serial.println(layer.getTimeline().getVolume(500));
}

void loop() {
  //statements
}
