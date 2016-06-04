#include "Controller.h"

namespace Zebra {

  Controller::Controller(Rhythm& rhythm_, View& view_, Player& player_)
  : rhythmRef(rhythm_)
  , viewRef(view_)
  , playerRef(player_)
  , keyboard()
  , selectedLayer(NULL)
  , selectedBeat(NULL)
  , selectedBeatNum(-1) {}

  Controller::~Controller() {}

  void Controller::initialize() {
    if (rhythmRef.getSelectActive()) {
      viewRef.drawInfoRhythmBase();
      viewRef.drawInfoRhythmAll();
    } else {
      viewRef.drawInfoLayerBase();
      viewRef.drawInfoLayerAll(*selectedLayer, *selectedBeat);
    }
    viewRef.resetPlayBar();
    viewRef.drawAllLayer();
  }

  void Controller::checkKeyboardStatus() {
    if (keyboard.rhythmSelectButton.checkStatus()) {
      rhythmSelectButtonPressed();
    }
    if (keyboard.layer0SelectButton.checkStatus()) {
      layerSelectButtonPressed(rhythmRef.getLayer(0));
    }
    if (keyboard.layer1SelectButton.checkStatus()) {
      layerSelectButtonPressed(rhythmRef.getLayer(1));
    }
    if (keyboard.layer2SelectButton.checkStatus()) {
      layerSelectButtonPressed(rhythmRef.getLayer(2));
    }
    if (keyboard.layer3SelectButton.checkStatus()) {
      layerSelectButtonPressed(rhythmRef.getLayer(3));
    }
    if (keyboard.rhythmTempoUpButton.checkStatus()) {
      if (rhythmRef.getSelectActive()) {
        rhythmTempoUpButtonPressed();
      } else {
        layerInstAUpButtonPressed();
      }
    }
    if (keyboard.rhythmTempoDownButton.checkStatus()) {
      if (rhythmRef.getSelectActive()) {
        rhythmTempoDownButtonPressed();
      } else {
        layerInstADownButtonPressed();
      }
    }
    if (keyboard.rhythmQuantizeUpButton.checkStatus()) {
      if (rhythmRef.getSelectActive()) {
        rhythmQuantizeUpButtonPressed();
      } else {
        layerInstBUpButtonPressed();
      }
    }
    if (keyboard.rhythmQuantizeDownButton.checkStatus()) {
      if (rhythmRef.getSelectActive()) {
        rhythmQuantizeDownButtonPressed();
      } else {
        layerInstBDownButtonPressed();
      }
    }
    if (keyboard.rhythmBarUpButton.checkStatus()) {
      if (rhythmRef.getSelectActive()) {
        rhythmBarUpButtonPressed();
      } else {
        beatUpButtonPressed();
      }
    }
    if (keyboard.rhythmBarDownButton.checkStatus()) {
      if (rhythmRef.getSelectActive()) {
        rhythmBarDownButtonPressed();
      } else {
        beatDownButtonPressed();
      }
    }
    if (keyboard.rhythmMeasureUpButton.checkStatus()) {
      if (rhythmRef.getSelectActive()) {
        rhythmMeasureUpButtonPressed();
      } else {
        fillUpButtonPressed();
      }
    }
    if (keyboard.rhythmMeasureDownButton.checkStatus()) {
      if (rhythmRef.getSelectActive()) {
        rhythmMeasureDownButtonPressed();
      } else {
        fillDownButtonPressed();
      }
    }
    if (keyboard.recordButton.checkStatus()) {
      recordButtonPressed();
    }
    if (keyboard.playButton.checkStatus()) {
      playButtonPressed();
    }
    if (keyboard.resetButton.checkStatus()) {
      resetButtonPressed();
    }
    if (keyboard.beatAButton.checkStatus()) {
      beatAButtonPressed();
    }
    if (keyboard.beatBButton.checkStatus()) {
      beatBButtonPressed();
    }
    if (keyboard.beatClearButton.checkStatus()) {
      beatClearButtonPressed();
    }
    if (keyboard.metronomeButton.checkStatus()) {
      metronomeButtonPressed();
    }
  }

  // play functions

  void Controller::recordButtonPressed() {
    if (selectedLayer == NULL) {
      layerSelectButtonPressed(rhythmRef.getLayer(0));
    }
    playerRef.record();
  }

  void Controller::playButtonPressed() {
    if (!playerRef.getPlayActive()) {
      playerRef.play();
    } else {
      playerRef.stop();
    }
  }

  void Controller::resetButtonPressed() {
    playerRef.reset();
    viewRef.resetPlayBar();
  }

  // select functions

  void Controller::rhythmSelectButtonPressed() {
    if (selectedLayer != NULL) {
      for (int i = 0; i < kLayerLibrarySize; i++) {
        rhythmRef.getLayer(i).setSelectActive(false);
        viewRef.drawLayerSelectActive(rhythmRef.getLayer(i));
      }
      rhythmRef.setSelectActive(true);
      selectedLayer = NULL;
      selectedBeat = NULL;
      selectedBeatNum = -1;
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      viewRef.drawRhythmSelectActive();
      viewRef.switchInfoFromLayerToRhythm();
    }
  }

  void Controller::layerSelectButtonPressed(Layer& layer_) {
    if (selectedLayer == NULL) {
      rhythmRef.setSelectActive(false);
      selectedLayer = &layer_;
      selectedLayer->setSelectActive(true);
      if (selectedLayer->getLastActiveBeat() >= 0) {
        selectedBeatNum = 0;
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      } else {
        selectedBeatNum = -1;
        selectedBeat = NULL;
      }
      viewRef.drawRhythmSelectActive();
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      viewRef.drawLayerSelectActive(*selectedLayer);
      viewRef.switchInfoFromRhythmToLayer(*selectedLayer);
    } else if (layer_.getNumber() != selectedLayer->getNumber()) {
      selectedLayer->setSelectActive(false);
      viewRef.drawLayerSelectActive(*selectedLayer);
      selectedLayer = &layer_;
      selectedLayer->setSelectActive(true);
      if (selectedLayer->getLastActiveBeat() >= 0) {
        selectedBeatNum = 0;
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      } else {
        selectedBeatNum = -1;
        selectedBeat = NULL;
      }
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      viewRef.drawLayerSelectActive(*selectedLayer);
      viewRef.switchInfoFromLayerToLayer(*selectedLayer);
    }
  }

  void Controller::layerBeatButtonPressed(Layer& layer_) {
    layer_.setBeatActive(!layer_.getBeatActive());
    viewRef.drawLayerBeatActive(layer_);
  }

  void Controller::layerFillButtonPressed(Layer& layer_) {
    layer_.setFillActive(!layer_.getFillActive());
    viewRef.drawLayerFillActive(layer_);
  }

  // rhythm functions

  void Controller::rhythmTempoUpButtonPressed() {
    uint8_t rhythmTempo = rhythmRef.getTempo();
    if (rhythmRef.getTempo() < kMaxRhythmTempo) {
      rhythmRef.setTempo(rhythmTempo + 1);
      playerRef.calculateAndStartTimer();
      viewRef.drawInfoRhythmTempo();
    }
  }

  void Controller::rhythmTempoDownButtonPressed() {
    uint8_t rhythmTempo = rhythmRef.getTempo();
    if (rhythmTempo > kMinRhythmTempo) {
      rhythmRef.setTempo(rhythmTempo - 1);
      playerRef.calculateAndStartTimer();
      viewRef.drawInfoRhythmTempo();
    }
  }

  void Controller::rhythmQuantizeUpButtonPressed() {
    uint8_t rhythmQuantize = rhythmRef.getQuantize();
    if (rhythmQuantize < kMaxRhythmQuantize) {
      rhythmRef.setQuantize(rhythmQuantize + 1);
      viewRef.drawInfoRhythmQuantize();
    }
  }

  void Controller::rhythmQuantizeDownButtonPressed() {
    uint8_t rhythmQuantize = rhythmRef.getQuantize();
    if (rhythmQuantize > kMinRhythmQuantize) {
      rhythmRef.setQuantize(rhythmQuantize - 1);
      viewRef.drawInfoRhythmQuantize();
    }
  }

  void Controller::rhythmBarUpButtonPressed() {
    uint8_t rhythmBar = rhythmRef.getBar();
    if (rhythmBar < kMaxRhythmBar) {
      playerRef.reset();
      rhythmRef.setBar(rhythmBar + 1);
      adjustBarUpTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawInfoRhythmBar();
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::rhythmBarDownButtonPressed() {
    uint8_t rhythmBar = rhythmRef.getBar();
    if (rhythmBar > kMinRhythmBar) {
      playerRef.reset();
      rhythmRef.setBar(rhythmBar - 1);
      adjustBarDownTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawInfoRhythmBar();
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::rhythmMeasureUpButtonPressed() {
    uint8_t rhythmMeasure = rhythmRef.getMeasure();
    if (rhythmMeasure < kMaxRhythmMeasure) {
      playerRef.reset();
      rhythmRef.setMeasure(rhythmMeasure + 1);
      adjustMeasureUpTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawInfoRhythmMeasure();
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::rhythmMeasureDownButtonPressed() {
    uint8_t rhythmMeasure = rhythmRef.getMeasure();
    if (rhythmMeasure > kMinRhythmMeasure) {
      playerRef.reset();
      rhythmRef.setMeasure(rhythmMeasure - 1);
      adjustMeasureDownTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawInfoRhythmMeasure();
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  // layer functions

  void Controller::layerInstAUpButtonPressed() {
    uint8_t layerInst = selectedLayer->getInstA();
    if (layerInst < kMaxLayerInst) {
      selectedLayer->setInstA(layerInst + 1);
      viewRef.drawInfoLayerInstA(*selectedLayer);
    }
  }

  void Controller::layerInstADownButtonPressed() {
    uint8_t layerInst = selectedLayer->getInstA();
    if (layerInst > kMinLayerInst) {
      selectedLayer->setInstA(layerInst - 1);
      viewRef.drawInfoLayerInstA(*selectedLayer);
    }
  }

  void Controller::layerInstBUpButtonPressed() {
    uint8_t layerInst = selectedLayer->getInstB();
    if (layerInst < kMaxLayerInst) {
      selectedLayer->setInstB(layerInst + 1);
      viewRef.drawInfoLayerInstB(*selectedLayer);
    }
  }

  void Controller::layerInstBDownButtonPressed() {
    uint8_t layerInst = selectedLayer->getInstB();
    if (layerInst > kMinLayerInst) {
      selectedLayer->setInstB(layerInst - 1);
      viewRef.drawInfoLayerInstB(*selectedLayer);
    }
  }

  // fill functions

  void Controller::beatUpButtonPressed() {
    // checking if layer is not empty
    if (selectedLayer->getLastActiveBeat() != -1) {
      if (selectedBeatNum < selectedLayer->getLastActiveBeat()) {
        selectedBeatNum += 1;
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
        viewRef.drawInfoFill(*selectedBeat);
        viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      } else if (selectedLayer->getLastActiveBeat() != 0) {
        selectedBeatNum = 0;
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
        viewRef.drawInfoFill(*selectedBeat);
        viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      }
    }
  }

  void Controller::beatDownButtonPressed() {
    // checking if layer is not empty
    if (selectedLayer->getLastActiveBeat() != -1) {
      if (selectedBeatNum > 0) {
        selectedBeatNum -= 1;
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
        viewRef.drawInfoFill(*selectedBeat);
        viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      } else if (selectedLayer->getLastActiveBeat() != 0) {
        selectedBeatNum = selectedLayer->getLastActiveBeat();
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
        viewRef.drawInfoFill(*selectedBeat);
        viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      }
    }
  }

  void Controller::fillUpButtonPressed() {
    // checking if layer is not empty
    if (selectedLayer->getLastActiveBeat() != -1) {
      uint8_t beatFill = selectedBeat->getFill();
      if (beatFill < (kFillLibrarySize - 1)) {
        selectedLayer->setFill(selectedBeatNum, beatFill + 1);
        viewRef.drawInfoFill(*selectedBeat);
        if (selectedBeat->getFill() == 1) {
          viewRef.drawBeatFill(*selectedLayer, selectedBeatNum, true);
        }
      }
    }
  }

  void Controller::fillDownButtonPressed() {
    // checking if layer is not empty
    if (selectedLayer->getLastActiveBeat() != -1) {
      uint8_t beatFill = selectedBeat->getFill();
      if (beatFill > 0) {
        selectedLayer->setFill(selectedBeatNum, beatFill - 1);
        viewRef.drawInfoFill(*selectedBeat);
        if (selectedBeat->getFill() == 0) {
          viewRef.drawBeatFill(*selectedLayer, selectedBeatNum, false);
        }
      }
    }
  }

  // beat record functions

  void Controller::beatAButtonPressed() {
    if (playerRef.getRecordActive() && !(rhythmRef.getSelectActive())) {
      uint32_t recordTime = rhythmRef.getPlayTime();
      bool recordInst = 0;
      selectedBeatNum = selectedLayer->setBeat(recordTime, 3, recordInst);
      selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      viewRef.drawLayerBeat(*selectedLayer, recordTime, recordInst);
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      viewRef.drawInfoFill(*selectedBeat);
    }
  }

  void Controller::beatBButtonPressed() {
    if (playerRef.getRecordActive() && !(rhythmRef.getSelectActive())) {
      uint32_t recordTime = rhythmRef.getPlayTime();
      bool recordInst = 1;
      selectedBeatNum = selectedLayer->setBeat(recordTime, 3, recordInst);
      selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      viewRef.drawLayerBeat(*selectedLayer, recordTime, recordInst);
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      viewRef.drawInfoFill(*selectedBeat);
    }
  }

  void Controller::beatClearButtonPressed() {
    if (!rhythmRef.getSelectActive()) {
      // clearing previous beat fill
      if (selectedBeatNum > 0) {
        viewRef.drawBeatFill(*selectedLayer, selectedBeatNum - 1, false);
      }
      // clearing selected beat's fill
      viewRef.drawBeatFill(*selectedLayer, selectedBeatNum, false);
      // clearing selected beat
      viewRef.clearLayerBeat(*selectedLayer, *selectedBeat);
      selectedLayer->clearBeat(selectedBeatNum);
      // shifting to new selected beat
      if ((selectedBeatNum == 0) && (selectedLayer->getLastActiveBeat() == -1)) {
        selectedBeatNum = -1;
        selectedBeat = NULL;
      } else if (selectedBeatNum > selectedLayer->getLastActiveBeat()) {
        selectedBeatNum = 0;
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      }
      // drawing new selected beat
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      // drawing new selected beat's fill
      if (selectedLayer->getLastActiveBeat() == -1) {
        viewRef.cleanInfoFill();
      } else {
        viewRef.drawInfoFill(*selectedBeat);
      }
    }
  }

  // metronome functions

  void Controller::metronomeButtonPressed() {}

  // private timing functions

  void Controller::adjustBarUpTiming() {
    // adjusting rhythm timing data
    rhythmRef.calculateSongTime();
  }

  void Controller::adjustBarDownTiming() {
    // adjusting rhythm timing data
    rhythmRef.calculateSongTime();
    // clearing beat if its time exceeds songtime
    for (int8_t i = kLayerLibrarySize - 1; i >= 0; i--) {
      for (int8_t j = kBeatLibrarySize - 1; j >= 0; j--) {
        Beat& beat = rhythmRef.getLayer(i).getBeat(j);
        if (beat.getActive()) {
          if (beat.getTime() >= rhythmRef.getSongTime()) {
            rhythmRef.getLayer(i).clearBeat(j);
          }
        }
      }
      rhythmRef.getLayer(i).calculateLastActiveBeat();
    }
  }

  void Controller::adjustMeasureUpTiming() {
    // adjusting rhythm timing data
    rhythmRef.calculateSongTime();
  }

  void Controller::adjustMeasureDownTiming() {
    // adjusting rhythm timing data
    rhythmRef.calculateSongTime();
    // clearing beat if its time exceeds songtime
    for (int8_t i = kLayerLibrarySize - 1; i >= 0; i--) {
      for (int8_t j = kBeatLibrarySize - 1; j >= 0; j--) {
        Beat& beat = rhythmRef.getLayer(i).getBeat(j);
        if (beat.getActive()) {
          if (beat.getTime() >= rhythmRef.getSongTime()) {
            rhythmRef.getLayer(i).clearBeat(j);
          }
        }
      }
      rhythmRef.getLayer(i).calculateLastActiveBeat();
    }
  }

  // private fill functions

  uint8_t Controller::getFillStep(uint8_t fillNum) {
    return pgm_read_byte(&fillStepLibrary[fillNum]);
  }

  char Controller::getFillName(uint8_t fillNum, uint8_t letterNum) {
    unsigned int flashAddress = pgm_read_word(&fillNameLibrary[fillNum]);
    return (char) pgm_read_byte(flashAddress + letterNum);
  }

  uint8_t Controller::getFillTime(uint8_t fillNum, uint8_t stepNum) {
    unsigned int flashAddress = pgm_read_word(&fillTimeLibrary[fillNum]);
    return pgm_read_byte(flashAddress + stepNum);
  }

  uint8_t Controller::getFillVolume(uint8_t fillNum, uint8_t stepNum) {
    unsigned int flashAddress = pgm_read_word(&fillVolumeLibrary[fillNum]);
    return pgm_read_byte(flashAddress + stepNum);
  }

  uint8_t Controller::getFillInst(uint8_t fillNum, uint8_t stepNum) {
    unsigned int flashAddress = pgm_read_word(&fillInstLibrary[fillNum]);
    return pgm_read_byte(flashAddress + stepNum);
  }
}
