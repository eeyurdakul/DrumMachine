#include "Controller.h"

namespace Zebra {

  Controller::Controller(Rhythm& rhythm_, View& view_, Player& player_)
  : rhythmRef(rhythm_)
  , viewRef(view_)
  , playerRef(player_)
  , keyboard()
  , selectedLayer(-1)
  , selectedBeat(-1) {}

  Controller::~Controller() {}

  void Controller::initialize() {
    ////////// test //////////
    for (uint8_t i = 0; i < 8; i++) {
      rhythmRef.getLayer(0).setBeat(64 * i, 3, (i % 2));
    }
    ////////// test //////////

    if (rhythmRef.getSelectActive()) {
      viewRef.drawInfoRhythmBase();
      viewRef.drawInfoRhythmAll();
    } else {
      viewRef.drawInfoLayerBase();
      viewRef.drawInfoLayerAll(rhythmRef.getLayer(selectedLayer), rhythmRef.getLayer(selectedLayer).getBeat(selectedBeat));
    }
    viewRef.resetPlayBar();
    viewRef.drawAllLayer();
  }

  void Controller::setSelectedLayer(int8_t selectedLayer_) {
    selectedLayer = selectedLayer_;
  }

  int8_t Controller::getSelectedLayer() const {
    return selectedLayer;
  }

  void Controller::setSelectedBeat(int8_t selectedBeat_) {
    selectedBeat = selectedBeat_;
  }

  int8_t Controller::getSelectedBeat() const {
    return selectedBeat;
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
      playerRef.record();
    }
    if (keyboard.playStopButton.checkStatus()) {
      if (!playerRef.getPlayActive()) {
        playerRef.play();
      } else {
        playerRef.stop();
      }
    }
    if (keyboard.resetButton.checkStatus()) {
      resetPlay();
    }
    if (keyboard.metronomeButton.checkStatus()) {
      // statements
    }
    if (keyboard.beatAButton.checkStatus()) {
      // statements
    }
    if (keyboard.beatBButton.checkStatus()) {
      // statements
    }
    if (keyboard.beatCButton.checkStatus()) {
      // statements
    }
    if (keyboard.beatDButton.checkStatus()) {
      // statements
    }
  }

  // select functions

  void Controller::rhythmSelectButtonPressed() {
    if (!rhythmRef.getSelectActive()) {
      for (int i = 0; i < kLayerLibrarySize; i++) {
        rhythmRef.getLayer(i).setSelectActive(false);
        viewRef.drawLayerSelectActive(rhythmRef.getLayer(i));
      }
      rhythmRef.setSelectActive(true);
      selectedLayer = -1;
      selectedBeat = -1;
      viewRef.drawSelectedFill(selectedLayer, selectedBeat);
      viewRef.drawRhythmSelectActive();
      viewRef.switchInfoFromLayerToRhythm();
    }
  }

  void Controller::layerSelectButtonPressed(Layer& layer_) {
    if (rhythmRef.getSelectActive()) {
      rhythmRef.setSelectActive(false);
      layer_.setSelectActive(true);
      selectedLayer = layer_.getNumber();
      selectedBeat = 0;
      viewRef.drawRhythmSelectActive();
      viewRef.drawSelectedFill(selectedLayer, selectedBeat);
      viewRef.drawLayerSelectActive(layer_);
      viewRef.switchInfoFromRhythmToLayer(selectedLayer);
    } else if (layer_.getNumber() != getSelectedLayer()) {
      rhythmRef.getLayer(selectedLayer).setSelectActive(false);
      viewRef.drawLayerSelectActive(rhythmRef.getLayer(selectedLayer));
      layer_.setSelectActive(true);
      selectedLayer = layer_.getNumber();
      selectedBeat = 0;
      viewRef.drawSelectedFill(selectedLayer, selectedBeat);
      viewRef.drawLayerSelectActive(layer_);
      viewRef.switchInfoFromLayerToLayer(selectedLayer);
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
      playerRef.setTimerMatchRegister();
      viewRef.drawInfoRhythmTempo();
    }
  }

  void Controller::rhythmTempoDownButtonPressed() {
    uint8_t rhythmTempo = rhythmRef.getTempo();
    if (rhythmTempo > kMinRhythmTempo) {
      rhythmRef.setTempo(rhythmTempo - 1);
      playerRef.setTimerMatchRegister();
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
      resetPlay();
      rhythmRef.setBar(rhythmBar + 1);
      adjustBarUpTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawInfoRhythmBar();
      viewRef.drawAllLayerMeasureAndSong();
    }
  }

  void Controller::rhythmBarDownButtonPressed() {
    uint8_t rhythmBar = rhythmRef.getBar();
    if (rhythmBar > kMinRhythmBar) {
      resetPlay();
      rhythmRef.setBar(rhythmBar - 1);
      adjustBarDownTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawInfoRhythmBar();
      viewRef.drawAllLayerMeasureAndSong();
    }
  }

  void Controller::rhythmMeasureUpButtonPressed() {
    uint8_t rhythmMeasure = rhythmRef.getMeasure();
    if (rhythmMeasure < kMaxRhythmMeasure) {
      resetPlay();
      rhythmRef.setMeasure(rhythmMeasure + 1);
      adjustMeasureUpTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawInfoRhythmMeasure();
      viewRef.drawAllLayerMeasureAndSong();
    }
  }

  void Controller::rhythmMeasureDownButtonPressed() {
    uint8_t rhythmMeasure = rhythmRef.getMeasure();
    if (rhythmMeasure > kMinRhythmMeasure) {
      resetPlay();
      rhythmRef.setMeasure(rhythmMeasure - 1);
      adjustMeasureDownTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawInfoRhythmMeasure();
      viewRef.drawAllLayerMeasureAndSong();
    }
  }

  // layer functions

  void Controller::layerInstAUpButtonPressed() {
    uint8_t layerInst = rhythmRef.getLayer(getSelectedLayer()).getInstA();
    if (layerInst < kMaxLayerInst) {
      rhythmRef.getLayer(getSelectedLayer()).setInstA(layerInst + 1);
      viewRef.drawInfoLayerInstA(rhythmRef.getLayer(selectedLayer));
    }
  }

  void Controller::layerInstADownButtonPressed() {
    uint8_t layerInst = rhythmRef.getLayer(getSelectedLayer()).getInstA();
    if (layerInst > kMinLayerInst) {
      rhythmRef.getLayer(getSelectedLayer()).setInstA(layerInst - 1);
      viewRef.drawInfoLayerInstA(rhythmRef.getLayer(selectedLayer));
    }
  }

  void Controller::layerInstBUpButtonPressed() {
    uint8_t layerInst = rhythmRef.getLayer(getSelectedLayer()).getInstB();
    if (layerInst < kMaxLayerInst) {
      rhythmRef.getLayer(getSelectedLayer()).setInstB(layerInst + 1);
      viewRef.drawInfoLayerInstB(rhythmRef.getLayer(selectedLayer));
    }
  }

  void Controller::layerInstBDownButtonPressed() {
    uint8_t layerInst = rhythmRef.getLayer(getSelectedLayer()).getInstB();
    if (layerInst > kMinLayerInst) {
      rhythmRef.getLayer(getSelectedLayer()).setInstB(layerInst - 1);
      viewRef.drawInfoLayerInstB(rhythmRef.getLayer(selectedLayer));
    }
  }

  // fill functions

  void Controller::beatUpButtonPressed() {
    // checking if layer is not empty
    if (rhythmRef.getLayer(selectedLayer).getLastActiveBeat() != -1) {
      if (selectedBeat < rhythmRef.getLayer(selectedLayer).getLastActiveBeat()) {
        selectedBeat += 1;
        viewRef.drawInfoFill(rhythmRef.getLayer(selectedLayer).getBeat(selectedBeat));
        viewRef.drawSelectedFill(selectedLayer, selectedBeat);
      }
    }
  }

  void Controller::beatDownButtonPressed() {
    // checking if layer is not empty
    if (rhythmRef.getLayer(selectedLayer).getLastActiveBeat() != -1) {
      if (selectedBeat > 0) {
        selectedBeat -= 1;
        viewRef.drawInfoFill(rhythmRef.getLayer(selectedLayer).getBeat(selectedBeat));
        viewRef.drawSelectedFill(selectedLayer, selectedBeat);
      }
    }
  }

  void Controller::fillUpButtonPressed() {
    // checking if layer is not empty
    if (rhythmRef.getLayer(selectedLayer).getLastActiveBeat() != -1) {
      Layer& layer = rhythmRef.getLayer(getSelectedLayer());
      Beat& beat = rhythmRef.getLayer(getSelectedLayer()).getBeat(getSelectedBeat());
      uint8_t beatFill = beat.getFill();
      if (beatFill < (kFillLibrarySize - 1)) {
        layer.setFill(getSelectedBeat(), beatFill + 1);
        viewRef.drawInfoFill(beat);
      }
    }
  }

  void Controller::fillDownButtonPressed() {
    // checking if layer is not empty
    if (rhythmRef.getLayer(selectedLayer).getLastActiveBeat() != -1) {
      Layer& layer = rhythmRef.getLayer(getSelectedLayer());
      Beat& beat = rhythmRef.getLayer(getSelectedLayer()).getBeat(getSelectedBeat());
      uint8_t beatFill = beat.getFill();
      if (beatFill > 0) {
        layer.setFill(getSelectedBeat(), beatFill - 1);
        viewRef.drawInfoFill(beat);
      }
    }
  }

  // private play functions

  void Controller::resetPlay() {
    // resetting all play functions
    playerRef.reset();
    viewRef.resetPlayBar();
  }

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
