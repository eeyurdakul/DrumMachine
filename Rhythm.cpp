#include "Rhythm.h"

namespace Zebra {

  Rhythm::Rhythm()
  : tempo(kInitialTempo)
  , quantize(kInitialQuantize)
  , bar(kInitialBar)
  , measure(kInitialMeasure)
  , selectActive(true)
  , songTime((kMeasureTime * kInitialMeasure) * kInitialBar)
  , layerLibrary {{0}, {1}, {2}, {3}} {}

  Rhythm::~Rhythm() {}

  void Rhythm::setTempo(uint8_t tempo_) {
    if ((tempo_ >= kMinRhythmTempo) && (tempo_ <= kMaxRhythmTempo)) {
      tempo = tempo_;
    }
  }

  uint8_t Rhythm::getTempo() const {
    return tempo;
  }

  void Rhythm::setQuantize(uint8_t quantize_) {
    if ((quantize_ >= kMinRhythmQuantize) && (quantize_ <= kMaxRhythmQuantize)) {
      quantize = quantize_;
    }
  }

  uint8_t Rhythm::getQuantize() const {
    return quantize;
  }

  void Rhythm::setBar(uint8_t bar_) {
    if ((bar_ >= kMinRhythmBar) && (bar_ <= kMaxRhythmBar)) {
      bar = bar_;
    }
  }

  uint8_t Rhythm::getBar() const {
    return bar;
  }

  void Rhythm::setMeasure(uint8_t measure_) {
    if ((measure_ >= kMinRhythmMeasure) && (measure_ <= kMaxRhythmMeasure)) {
      measure = measure_;
    }
  }

  uint8_t Rhythm::getMeasure() const {
    return measure;
  }

  void Rhythm::setSelectActive(bool active_) {
    selectActive = active_;
  }

  bool Rhythm::getSelectActive() const {
    return selectActive;
  }

  void Rhythm::setPlayTime(uint32_t playTime_) {
    playTime = playTime_;
  }

  uint32_t Rhythm::getPlayTime() const {
    return playTime;
  }

  void Rhythm::incrementPlayTime() {
    if (playTime < songTime) {
      playTime++;
    } else {
      playTime = 0;
    }
  }

  void Rhythm::calculateSongTime() {
    songTime = kMeasureTime * getMeasure() * getBar();
  }

  uint32_t Rhythm::getSongTime() const {
    return songTime;
  }

  Layer& Rhythm::getLayer(uint8_t layerNum) {
    return layerLibrary[layerNum];
  }
}
