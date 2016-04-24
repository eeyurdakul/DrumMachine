#include "Player.h"

namespace Zebra {

  Player::Player(Rhythm& rhythm_, View& view_)
  : rhythmRef(rhythm_)
  , viewRef(view_)
  , active(false)
  , time(0) {}

  Player::~Player() {}

  void Player::initialize() {
    pinMode(41, OUTPUT);
    calculateDelayRegister();
    // initializing timer1
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    OCR1A = 31;
    // turn on CTC mode
    TCCR1B |= (1 << WGM12);
    // Set CS12 and CS10 bits for 64 prescaler
    TCCR1B |= (1 << CS11) | (1 << CS10);
    // enable timer compare interrupt
    TIMSK1 |= (1 << OCIE1A);
    interrupts();
  }

  void Player::reset() {
    if (active) {
      stop();
    }
    time = 0;
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

  void Player::setTime(uint32_t time_) {
    time = time_;
  }

  uint32_t Player::getTime() const {
    return time;
  }

  void Player::calculateDelay() {
    delay = kMicroSecondsinOneMinute / (rhythmRef.getTempo() * kMeasureTime);
  }

  uint32_t Player::getDelay() const {
    return delay;
  }

  void Player::calculateDelayRegister() {
    calculateDelay();
    delayRegister = (delay / kTimerPreScaler) - 1;
  }

  uint32_t Player::getDelayRegister() const {
    return delay;
  }
}
