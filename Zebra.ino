#include "Layer.h"
#include "Timeframe.h"
#include "Timeline.h"
#include "Beat.h"

using namespace Zebra;

Layer layer(0);

void setup() {
  Serial.begin(9600);
  layer.setBeat(500, 2, 1);
  Serial.println(layer.getBeat(0).getTime());
  Serial.println(layer.getTimeline().getVolume(500));
  layer.setBeat(250, 1, 1);
  Serial.println(layer.getBeat(0).getTime());
  Serial.println(layer.getTimeline().getVolume(250));
  layer.reset();
  Serial.println(layer.getBeat(0).getTime());
  Serial.println(layer.getTimeline().getVolume(250));
}

void loop() {
  //statements
}
