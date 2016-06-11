#include "Rhythm.h"
#include "View.h"
#include "Controller.h"

using namespace Zebra;

Rhythm rhythm;
View view(rhythm);
Player player(rhythm, view);
Controller controller(rhythm, view, player);

void setup() {
  view.initialize();
  player.initialize();
  controller.initialize();
}

void loop() {
  controller.checkKeyboard();
  view.update();
}

void TC3_Handler()
{
  // accepting interrupt
  TC_GetStatus(TC1, 0);
  // function
  if (player.getPlayActive()) {
    playMidi();
    if (rhythm.getPlayTime() < rhythm.getSongTime()) {
      rhythm.incrementPlayTime();
    } else {
      rhythm.restartPlayTime();
      view.restartPlayBar();
    }
  }
}

void playMidi() {
  uint32_t playTime = rhythm.getPlayTime();
  // checking for each layer
  for (uint8_t i = 0; i < kLayerLibrarySize; i++) {
    // defining variables
    Timeline& timeline = rhythm.getLayer(i).getTimeline();
    uint8_t volume;
    bool type;
    uint8_t inst;
    // checking if there is an active beat/fill on timeline
    if (timeline.getVolume(playTime)) {
      // setting volume
      switch (timeline.getVolume(playTime)) {
        case 1:
        volume = kMidiVolume1;
        break;
        case 2:
        volume = kMidiVolume2;
        break;
        case 3:
        volume = kMidiVolume3;
        break;
        default:
        volume = kMidiVolume3;
        break;
      }
      // setting type
      type = timeline.getType(playTime);
      // setting inst
      if (timeline.getInst(playTime) == 0) {
        inst = rhythm.getLayer(i).getInstAMidi();
      } else {
        inst = rhythm.getLayer(i).getInstBMidi();
      }
      // playing midi
      if ((type == 0) && (rhythm.getLayer(i).getBeatActive())) {
        player.getMidi().noteOn(inst, volume);
      } else if ((type == 1) && (rhythm.getLayer(i).getFillActive())) {
        player.getMidi().noteOn(inst, volume);
      }
    }
  }
}
