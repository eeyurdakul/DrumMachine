#include "Player.h"

namespace Zebra {

  Player::Player(Rhythm& rhythm_, View& view_)
  : rhythmRef(rhythm_)
  , viewRef(view_)
  , playActive(false)
  , recordActive(false)
  , midiActive(false)
  , audioActive(true)
  , midi() {}

  Player::~Player() {}

  void Player::initialize() {
    midi.initialize();
    calculateAndStartTimer();
    pinMode(kRecordLedPin, OUTPUT);
    pinMode(kPlayLedPin, OUTPUT);
  }

  void Player::reset() {
    if (playActive) {
      stop();
    }
    rhythmRef.setPlayTime(0);
  }

  void Player::play() {
    playActive = true;
    digitalWrite(kPlayLedPin, HIGH);
  }

  void Player::record() {
    recordActive = true;
    playActive = true;
    digitalWrite(kRecordLedPin, HIGH);
    digitalWrite(kPlayLedPin, HIGH);
  }

  void Player::stop() {
    recordActive = false;
    playActive = false;
    digitalWrite(kRecordLedPin, LOW);
    digitalWrite(kPlayLedPin, LOW);
  }

  void Player::setPlayActive(bool active_) {
    playActive = active_;
  }

  bool Player::getPlayActive() const {
    return playActive;
  }

  void Player::setRecordActive(bool active_) {
    recordActive = active_;
  }

  bool Player::getRecordActive() const {
    return recordActive;
  }

  void Player::setMidiActive(bool active_) {
    midiActive = active_;
  }

  bool Player::getMidiActive() const {
    return midiActive;
  }

  void Player::setAudioActive(bool active_) {
    audioActive = active_;
  }

  bool Player::getAudioActive() const {
    return audioActive;
  }

  void Player::calculatePeriod() {
    period = double(kMicroSecondsinOneMinute) / rhythmRef.getTempo() / kMeasureTime;
  }

  double Player::getPeriod() const {
    return period;
  }

  void Player::calculateFrequency() {
    calculatePeriod();
    frequency = double(kMicroSecondsinOneSecond) / period;
  }

  double Player::getFrequency() const {
    return frequency;
  }

  void Player::calculateAndStartTimer() {
    calculateFrequency();
    // TC --> 1 , Channel --> 0
    pmc_set_writeprotect(false);
    pmc_enable_periph_clk((uint32_t)TC3_IRQn);
    TC_Configure(TC1, 0, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
    uint32_t rc = uint32_t(double(VARIANT_MCK) / 128 / frequency);
    TC_SetRC(TC1, 0, rc);
    TC_Start(TC1, 0);
    TC1->TC_CHANNEL[0].TC_IER=TC_IER_CPCS;
    TC1->TC_CHANNEL[0].TC_IDR=~TC_IER_CPCS;
    NVIC_EnableIRQ(TC3_IRQn);
  }

  Midi& Player::getMidi() {
    return midi;
  }
}
