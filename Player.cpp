#include "Player.h"

namespace Zebra {

  Player::Player(Rhythm& rhythm_, View& view_)
  : rhythmRef(rhythm_)
  , viewRef(view_)
  , active(false) {}

  Player::~Player() {}

  void Player::initialize() {
    pinMode(41, OUTPUT);
    calculateMatchRegister();
    // initializing timer1
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    OCR1A = matchRegister;
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12 and CS10 bits for 1024 prescaler
    TCCR1B |= (1 << CS12) | (1 << CS10);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    interrupts();
  }

  void Player::reset() {
    if (active) {
      stop();
    }
    rhythmRef.setPlayTime(0);
  }

  void Player::play() {
    active = true;
    digitalWrite(41, HIGH);
  }

  void Player::stop() {
    active = false;
    digitalWrite(41, LOW);
  }

  void Player::setActive(bool active_) {
    active = active_;
  }

  bool Player::getActive() const {
    return active;
  }

  void Player::calculatePeriod() {
    period = kMicroSecondsinOneMinute / (rhythmRef.getTempo() * kMeasureTime);
  }

  uint32_t Player::getPeriod() const {
    return period;
  }

  void Player::calculateFrequency() {
    calculatePeriod();
    frequency = kMicroSecondsinOneSecond / period;
  }

  uint32_t Player::getFrequency() const {
    return frequency;
  }

  void Player::calculateMatchRegister() {
    calculateFrequency();
    matchRegister = (16000000 / (kTimerPreScaler * frequency)) - 1;
  }

  uint32_t Player::getMatchRegister() const {
    return matchRegister;
  }

  void Player::setTimerMatchRegister() {
    calculateMatchRegister();
    noInterrupts();
    OCR1A = matchRegister;
    interrupts();
  }
}
