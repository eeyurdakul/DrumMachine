#include "Controller.h"

namespace Zebra {

  Controller::Controller(Rhythm& rhythm_, View& view_, Player& player_)
  : rhythmRef(rhythm_)
  , viewRef(view_)
  , playerRef(player_)
  , keyboard()
  , selectedLayer(NULL)
  , selectedBeat(NULL)
  , selectedBeatNum(-1)
  , currentMenuState(0)
  , previousMenuState(-1) {}

  Controller::~Controller() {}

  void Controller::initialize() {
    if (rhythmRef.getSelected()) {

      viewRef.drawInfoRhythmBase();
      //viewRef.drawInfoRhythmAll();
    } else {
      viewRef.drawInfoLayerBase();
      viewRef.drawInfoLayerAll(*selectedLayer, *selectedBeat);
    }
    viewRef.resetPlayBar();
    viewRef.drawAllLayer();
  }

  void Controller::checkKeyboard() {
    if (keyboard.menuSelectButton.checkStatus()) {
      menuSelect();
    }
    if (keyboard.layer0SelectButton.checkStatus()) {
      layerSelect(rhythmRef.getLayer(0));
    }
    if (keyboard.layer1SelectButton.checkStatus()) {
      layerSelect(rhythmRef.getLayer(1));
    }
    if (keyboard.layer2SelectButton.checkStatus()) {
      layerSelect(rhythmRef.getLayer(2));
    }
    if (keyboard.layer3SelectButton.checkStatus()) {
      layerSelect(rhythmRef.getLayer(3));
    }
    if (keyboard.upButton.checkStatus()) {
      if (currentMenuState >= 0) {
        menuUp();
      } else {
        fillUp();
      }
    }
    if (keyboard.downButton.checkStatus()) {
      if (currentMenuState >= 0) {
        menuDown();
      } else {
        fillDown();
      }
    }
    if (keyboard.rightButton.checkStatus()) {
      if (currentMenuState >= 0) {
        menuRight();
      } else {
        beatUp();
      }
    }
    if (keyboard.leftButton.checkStatus()) {
      if (currentMenuState >= 0) {
        menuLeft();
      } else {
        beatDown();
      }
    }
    if (keyboard.instAUpButton.checkStatus()) {
      if (currentMenuState == -1) {
        instAUp();
      }
    }
    if (keyboard.instADownButton.checkStatus()) {
      if (currentMenuState == -1) {
        instADown();
      }
    }
    if (keyboard.instBUpButton.checkStatus()) {
      if (currentMenuState == -1) {
        instBUp();
      }
    }
    if (keyboard.instBDownButton.checkStatus()) {
      if (currentMenuState == -1) {
        instBDown();
      }
    }
    if (keyboard.recordButton.checkStatus()) {
      record();
    }
    if (keyboard.playButton.checkStatus()) {
      playStop();
    }
    if (keyboard.resetButton.checkStatus()) {
      reset();
    }
    if (keyboard.beatAButton.checkStatus()) {
      beatA();
    }
    if (keyboard.beatBButton.checkStatus()) {
      beatB();
    }
    if (keyboard.beatClearButton.checkStatus()) {
      beatClear();
    }
  }

  // play functions

  void Controller::record() {
    if (selectedLayer == NULL) {
      layerSelectButtonPressed(rhythmRef.getLayer(0));
    }
    playerRef.record();
  }

  void Controller::playStop() {
    if (!playerRef.getPlayActive()) {
      playerRef.play();
    } else {
      playerRef.stop();
    }
  }

  void Controller::reset() {
    playerRef.reset();
    viewRef.resetPlayBar();
  }

  // select functions

  void Controller::menuSelect() {
    if (currentMenuState == -1) {
      for (uint8_t i = 0; i < kLayerLibrarySize; i++) {
        rhythmRef.getLayer(i).setSelected(false);
        viewRef.drawLayerSelected(rhythmRef.getLayer(i));
      }
      currentMenuState = 0;
      previousMenuState = -1;
      selectedLayer = NULL;
      selectedBeat = NULL;
      selectedBeatNum = -1;
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
    }
  }

  void Controller::layerSelect(Layer& layer_) {
    if (selectedLayer == NULL) {
      currentMenuState = -1;
      previousMenuState = -1;
      selectedLayer = &layer_;
      selectedLayer->setSelected(true);
      if (selectedLayer->getLastActiveBeat() >= 0) {
        selectedBeatNum = 0;
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      } else {
        selectedBeatNum = -1;
        selectedBeat = NULL;
      }
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      viewRef.drawLayerSelected(*selectedLayer);
      viewRef.switchToLayer(*selectedLayer);
    } else if (layer_.getNumber() != selectedLayer->getNumber()) {
      selectedLayer->setSelected(false);
      viewRef.drawLayerSelected(*selectedLayer);
      selectedLayer = &layer_;
      selectedLayer->setSelected(true);
      if (selectedLayer->getLastActiveBeat() >= 0) {
        selectedBeatNum = 0;
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      } else {
        selectedBeatNum = -1;
        selectedBeat = NULL;
      }
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      viewRef.drawLayerSelected(*selectedLayer);
      viewRef.switchBetweenLayers(*selectedLayer);
    }
  }

  void Controller::layerChannelSelect(Layer& layer_) {
    layer_.setBeatActive(!layer_.getBeatActive());
    viewRef.drawLayerBeatActive(layer_);
  }

  // menu functions

  void Controller::menuRight() {
    previousMenuState = currentMenuState;
    if (currentMenuState < kMaxMenuState) {
      currentMenuState += 1;
    } else {
      currentMenuState = 0;
    }
    menuUpdate();
  }

  void Controller::menuLeft() {
    previousMenuState = currentMenuState;
    if (currentMenuState > 0) {
      currentMenuState -= 1;
    } else {
      currentMenuState = kMaxMenuState;
    }
    menuUpdate();
  }

  void Controller::menuUp() {
    switch (currentMenuState) {
      case 0:
      tempoUp();
      break;
      case 1:
      metronomeUp();
      break;
      case 2:
      barUp();
      break;
      case 3:
      measureUp();
      break;
      case 4:
      loadUp();
      break;
      case 5:
      saveUp();
      break;
      case 6:
      outputUp();
      break;
      case 7:
      quantizeUp();
      break;
      default:
      break;
    }
  }

  void Controller::menuDown() {
    switch (currentMenuState) {
      case 0:
      tempoDown();
      break;
      case 1:
      metronomeDown();
      break;
      case 2:
      barDown();
      break;
      case 3:
      measureDown();
      break;
      case 4:
      loadDown();
      break;
      case 5:
      saveDown();
      break;
      case 6:
      outputDown();
      break;
      case 7:
      quantizeDown();
      break;
      default:
      break;
    }
  }

  void Controller::menuUpdate() {
    switch (currentMenuState) {
      case 0:   // tempo
      if ((previousMenuState == -1) || (previousMenuState == 7)) {
        drawTempo(1);
        drawMetronome(0);
        drawBar(0);
        drawMeasure(0);
      } else if (previousMenuState == 1) {
        drawTempo(1);
        drawMetronome(0);
      }
      break;
      case 1:   // metronome
      if (previousMenuState == 0) {
        drawTempo(0);
        drawMetronome(1);
      } else if (previousMenuState == 2) {
        drawMetronome(1);
        drawBar(0);
      }
      break;
      case 2:   // bar
      if (previousMenuState == 1) {
        drawMetronome(0);
        drawBar(1);
      } else if (previousMenuState == 3) {
        drawBar(1);
        drawMeasure(0);
      }
      break;
      case 3:   // measure
      if (previousMenuState == 2) {
        drawBar(0);
        drawMeasure(1);
      } else if (previousMenuState == 4) {
        drawTempo(0);
        drawMetronome(0);
        drawBar(0);
        drawMeasure(1);
      }
      break;
      case 4:   // load
      if (previousMenuState == 3) {
        drawLoad(1);
        drawSave(0);
        drawOutput(0);
        drawQuantize(0);
      } else if (previousMenuState == 5) {
        drawLoad(1);
        drawSave(0);
      }
      break;
      case 5:   // save
      if (previousMenuState == 4) {
        drawLoad(0);
        drawSave(1);
      } else if (previousMenuState == 6) {
        drawSave(1);
        drawOutput(0);
      }
      break;
      case 6:   // output
      if (previousMenuState == 5) {
        drawSave(0);
        drawOutput(1);
      } else if (previousMenuState == 7) {
        drawOutput(1);
        drawQuantize(0);
      }
      break;
      case 7:   // quantize
      if (previousMenuState == 6) {
        drawOutput(0);
        drawQuantize(1);
      } else if (previousMenuState == 0) {
        drawLoad(0);
        drawSave(0);
        drawOutput(0);
        drawQuantize(1);
      }
      break;
      default:
      break;
    }
  }

  // rhythm functions

  void Controller::tempoUp() {
    uint8_t rhythmTempo = rhythmRef.getTempo();
    if (rhythmRef.getTempo() < kMaxRhythmTempo) {
      rhythmRef.setTempo(rhythmTempo + 1);
      playerRef.calculateAndStartTimer();
      viewRef.drawTempoData();
    }
  }

  void Controller::tempoDown() {
    uint8_t rhythmTempo = rhythmRef.getTempo();
    if (rhythmTempo > kMinRhythmTempo) {
      rhythmRef.setTempo(rhythmTempo - 1);
      playerRef.calculateAndStartTimer();
      viewRef.drawTempoData();
    }
  }

  void Controller::metronomeUp() {}

  void Controller::metronomeDown() {}

  void Controller::barUp() {
    uint8_t rhythmBar = rhythmRef.getBar();
    if (rhythmBar < kMaxRhythmBar) {
      playerRef.reset();
      rhythmRef.setBar(rhythmBar + 1);
      adjustBarUpTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawBarData();
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::barDown() {
    uint8_t rhythmBar = rhythmRef.getBar();
    if (rhythmBar > kMinRhythmBar) {
      playerRef.reset();
      rhythmRef.setBar(rhythmBar - 1);
      adjustBarDownTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawBarData();
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::measureUp() {
    uint8_t rhythmMeasure = rhythmRef.getMeasure();
    if (rhythmMeasure < kMaxRhythmMeasure) {
      playerRef.reset();
      rhythmRef.setMeasure(rhythmMeasure + 1);
      adjustMeasureUpTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawMeasureData();
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::measureDown() {
    uint8_t rhythmMeasure = rhythmRef.getMeasure();
    if (rhythmMeasure > kMinRhythmMeasure) {
      playerRef.reset();
      rhythmRef.setMeasure(rhythmMeasure - 1);
      adjustMeasureDownTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawMeasureData();
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::loadUp() {}

  void Controller::loadDown() {}

  void Controller::saveUp() {}

  void Controller::saveDown() {}

  void Controller::outputUp() {}

  void Controller::outputDown() {}

  void Controller::quantizeUp() {
    uint8_t rhythmQuantize = rhythmRef.getQuantize();
    if (rhythmQuantize < kMaxRhythmQuantize) {
      rhythmRef.setQuantize(rhythmQuantize + 1);
      viewRef.drawQuantizeData();
    }
  }

  void Controller::quantizeDown() {
    uint8_t rhythmQuantize = rhythmRef.getQuantize();
    if (rhythmQuantize > kMinRhythmQuantize) {
      rhythmRef.setQuantize(rhythmQuantize - 1);
      viewRef.drawQuantizeData();
    }
  }

  // layer functions

  void Controller::instAUp() {
    uint8_t layerInst = selectedLayer->getInstAMidi();
    if (layerInst < kMaxLayerInstMidi) {
      selectedLayer->setInstAMidi(layerInst + 1);
      viewRef.drawInstA(*selectedLayer);
    }
  }

  void Controller::instADown() {
    uint8_t layerInst = selectedLayer->getInstAMidi();
    if (layerInst > kMinLayerInstMidi) {
      selectedLayer->setInstAMidi(layerInst - 1);
      viewRef.drawInstA(*selectedLayer);
    }
  }

  void Controller::instBUp() {
    uint8_t layerInst = selectedLayer->getInstBMidi();
    if (layerInst < kMaxLayerInstMidi) {
      selectedLayer->setInstBMidi(layerInst + 1);
      viewRef.drawInstB(*selectedLayer);
    }
  }

  void Controller::instBDown() {
    uint8_t layerInst = selectedLayer->getInstBMidi();
    if (layerInst > kMinLayerInstMidi) {
      selectedLayer->setInstBMidi(layerInst - 1);
      viewRef.drawInstB(*selectedLayer);
    }
  }

  // fill functions

  void Controller::beatUp() {
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

  void Controller::beatDown() {
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

  void Controller::fillUp() {
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

  void Controller::fillDown() {
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

  void Controller::beatA() {
    if (playerRef.getRecordActive() && !(rhythmRef.getSelected())) {
      uint32_t recordTime = rhythmRef.getPlayTime();
      bool recordInst = 0;
      selectedBeatNum = selectedLayer->setBeat(recordTime, 3, recordInst);
      selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      viewRef.drawLayerBeat(*selectedLayer, recordTime, recordInst);
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      viewRef.drawInfoFill(*selectedBeat);
    }
  }

  void Controller::beatB() {
    if (playerRef.getRecordActive() && !(rhythmRef.getSelected())) {
      uint32_t recordTime = rhythmRef.getPlayTime();
      bool recordInst = 1;
      selectedBeatNum = selectedLayer->setBeat(recordTime, 3, recordInst);
      selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      viewRef.drawLayerBeat(*selectedLayer, recordTime, recordInst);
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      viewRef.drawInfoFill(*selectedBeat);
    }
  }

  void Controller::beatClear() {
    if (!rhythmRef.getSelected()) {
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
