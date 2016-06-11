#include "Midi.h"

namespace Zebra {

  Midi::Midi()
  : channel(kMidiInitialChannel) {}

  Midi::~Midi() {}

  void Midi::initialize() {
    Serial.begin(kMidiBaudRate);
  }

  void Midi::setChannel(uint8_t channel_) {
    if (channel_ < 16) {
      channel = channel_;
    }
  }

  uint8_t Midi::getChannel() const {
    return channel;
  }

  void Midi::noteOn(uint8_t pitch, uint8_t velocity) {
    if ((pitch < 128) && (velocity < 128)) {
      Serial.write(0x90 + channel);
      Serial.write(pitch);
      Serial.write(velocity);
    }
  }

  void Midi::noteOff(uint8_t pitch, uint8_t velocity) {
    if ((pitch < 128) && (velocity < 128)) {
      Serial.write(0x80 + channel);
      Serial.write(pitch);
      Serial.write(velocity);
    }
  }
}
