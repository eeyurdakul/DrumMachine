#include "Controller.h"

namespace Zebra {

  Controller::Controller(Rhythm& rhythm_, View& view_, Player& player_)
  : rhythmRef(rhythm_)
  , viewRef(view_)
  , playerRef(player_)
  , keyboard()
  , selectedLayer(NULL)
  , selectedBeat(NULL)
  , fillEditMode(false)
  , selectedBeatNum(-1)
  , currentRhythmMenu(0)
  , previousRhythmMenu(-1)
  , currentLayerMenu(-1)
  , previousLayerMenu(-1) {}

  Controller::~Controller() {}

  void Controller::initialize() {
    menuUpdate();
    viewRef.resetPlayBar();
    viewRef.drawAllLayer();
  }

  void Controller::checkKeyboard() {
    // select buttons
    if (keyboard.rhythmSelectButton.checkStatus()) {
      rhythmSelect();
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
    // global select button
    if (keyboard.selectButton.checkStatus()) {
      select();
    }
    // channel buttons
    if (keyboard.layer0ChannelButton.checkStatus()) {
      layerChannelSelect(rhythmRef.getLayer(0));
    }
    if (keyboard.layer1ChannelButton.checkStatus()) {
      layerChannelSelect(rhythmRef.getLayer(1));
    }
    if (keyboard.layer2ChannelButton.checkStatus()) {
      layerChannelSelect(rhythmRef.getLayer(2));
    }
    if (keyboard.layer3ChannelButton.checkStatus()) {
      layerChannelSelect(rhythmRef.getLayer(3));
    }
    // menu buttons
    if (keyboard.upButton.checkStatus()) {
      if ((currentRhythmMenu >= 0) && (currentLayerMenu < 0)) {
        rhythmMenuUp();
      } else if ((currentLayerMenu >= 0) && (currentRhythmMenu < 0)) {
        if (fillEditMode) {
          fillUp();
        } else {
          layerMenuUp();
        }
      }
    }
    if (keyboard.downButton.checkStatus()) {
      if ((currentRhythmMenu >= 0) && (currentLayerMenu < 0)) {
        rhythmMenuDown();
      } else if ((currentLayerMenu >= 0) && (currentRhythmMenu < 0)) {
        if (fillEditMode) {
          fillDown();
        } else {
          layerMenuDown();
        }
      }
    }
    if (keyboard.rightButton.checkStatus()) {
      if ((currentRhythmMenu >= 0) && (currentLayerMenu < 0)) {
        rhythmMenuRight();
      } else if ((currentLayerMenu >= 0) && (currentRhythmMenu < 0)) {
        if (fillEditMode) {
          beatUp();
        } else {
          layerMenuRight();
        }
      }
    }
    if (keyboard.leftButton.checkStatus()) {
      if ((currentRhythmMenu >= 0) && (currentLayerMenu < 0)) {
        rhythmMenuLeft();
      } else if ((currentLayerMenu >= 0) && (currentRhythmMenu < 0)) {
        if (fillEditMode) {
          beatDown();
        } else {
          layerMenuLeft();
        }
      }
    }
    // action buttons
    if (keyboard.recordButton.checkStatus()) {
      record();
    }
    if (keyboard.playButton.checkStatus()) {
      playStop();
    }
    if (keyboard.resetButton.checkStatus()) {
      reset();
    }
    // beat buttons
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
      layerSelect(rhythmRef.getLayer(0));
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

  void Controller::rhythmSelect() {
    if (selectedLayer != NULL) {
      for (int i = 0; i < kLayerLibrarySize; i++) {
        rhythmRef.getLayer(i).setSelected(false);
        viewRef.drawLayerSelected(rhythmRef.getLayer(i));
      }
      // rhythm menu settings
      currentRhythmMenu = 0;
      // layer menu settings
      currentLayerMenu = -1;
      previousLayerMenu = -1;
      fillEditMode = false;
      rhythmRef.setSelected(true);
      selectedLayer = NULL;
      selectedBeat = NULL;
      selectedBeatNum = -1;
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      menuUpdate();
    }
  }

  void Controller::layerSelect(Layer& layer_) {
    // switching from rhythm to layer
    if (selectedLayer == NULL) {
      // rhythm menu settings
      currentRhythmMenu = -1;
      previousRhythmMenu = -1;
      rhythmRef.setSelected(false);
      // layer menu settings
      currentLayerMenu = 0;
      fillEditMode = true;
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
      menuUpdate();
    } // switching from layer to layer
    else if (layer_.getNumber() != selectedLayer->getNumber()) {
      selectedLayer->setSelected(false);
      viewRef.drawLayerSelected(*selectedLayer);
      selectedLayer = &layer_;
      selectedLayer->setSelected(true);
      // layer menu settings
      currentLayerMenu = 0;
      fillEditMode = true;
      if (selectedLayer->getLastActiveBeat() >= 0) {
        selectedBeatNum = 0;
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      } else {
        selectedBeatNum = -1;
        selectedBeat = NULL;
      }
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      viewRef.drawLayerSelected(*selectedLayer);
      menuUpdate();
    } // switching fill edit mode
    else {
      if ((currentLayerMenu == 0) && (fillEditMode)) {
        fillEditMode = false;
        currentLayerMenu = 1;
        menuUpdate();
      } else if ((currentLayerMenu > 0) &&(!fillEditMode)) {
        fillEditMode = true;
        currentLayerMenu = 0;
        menuUpdate();
      }
    }

  }

  void Controller::layerChannelSelect(Layer& layer_) {
    if ((layer_.getBeatActive()) && (layer_.getFillActive())) {
      layer_.setFillActive(false);
      viewRef.drawLayerFillActive(layer_);
    } else if (layer_.getBeatActive()) {
      layer_.setBeatActive(false);
      viewRef.drawLayerBeatActive(layer_);
    } else {
      layer_.setBeatActive(true);
      layer_.setFillActive(true);
      viewRef.drawLayerBeatActive(layer_);
      viewRef.drawLayerFillActive(layer_);
    }
  }

  // global select function

  void Controller::select() {}

  // menu function

  void Controller::menuUpdate() {
    if ((currentRhythmMenu >= 0) && (currentLayerMenu < 0)) {
      // rhythm menu update
      if ((currentRhythmMenu == 0) && (previousRhythmMenu == -1)) {
        viewRef.drawRhythmMenu1();
      } else if ((currentRhythmMenu == 3) && (previousRhythmMenu == 4)) {
        viewRef.drawRhythmMenu1();
      } else if ((currentRhythmMenu == 4) && (previousRhythmMenu == 3)) {
        viewRef.drawRhythmMenu2();
      }
      // drawing menu selection
      viewRef.drawRhythmMenuSelection(currentRhythmMenu, previousRhythmMenu);
    } else if ((currentLayerMenu >= 0) && (currentRhythmMenu < 0)) {
      // layer menu update
      // checking if switching from rhythm to layer
      if ((currentLayerMenu == 0) && (previousLayerMenu == -1)) {
        // drawing layer menu
        viewRef.drawLayerMenu(*selectedLayer, *selectedBeat);
        // drawing menu selection
        viewRef.drawLayerMenuSelection(currentLayerMenu, previousLayerMenu);
        // updating previousLayerMenu
        previousLayerMenu = 0;
      } else {
        // checking if switching between layers
        viewRef.switchBetweenLayers(*selectedLayer, *selectedBeat, previousLayerMenu);
        // checking if menu selection is changed
        if (currentLayerMenu != previousLayerMenu) {
          // drawing menu selection
          viewRef.drawLayerMenuSelection(currentLayerMenu, previousLayerMenu);
          // updating previousLayerMenu
          previousLayerMenu = currentLayerMenu;
        }
      }
    }
  }

  // rhythm menu functions

  void Controller::rhythmMenuRight() {
    if (currentRhythmMenu < kMaxRhythmMenu) {
      previousRhythmMenu = currentRhythmMenu;
      currentRhythmMenu += 1;
      menuUpdate();
    }
  }

  void Controller::rhythmMenuLeft() {
    if (currentRhythmMenu > 0) {
      previousRhythmMenu = currentRhythmMenu;
      currentRhythmMenu -= 1;
      menuUpdate();
    }
  }

  void Controller::rhythmMenuUp() {
    switch (currentRhythmMenu) {
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

  void Controller::rhythmMenuDown() {
    switch (currentRhythmMenu) {
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

  // rhythm functions

  void Controller::tempoUp() {
    uint8_t rhythmTempo = rhythmRef.getTempo();
    if (rhythmRef.getTempo() < kMaxTempo) {
      rhythmRef.setTempo(rhythmTempo + 1);
      playerRef.calculateAndStartTimer();
      viewRef.drawTempo();
    }
  }

  void Controller::tempoDown() {
    uint8_t rhythmTempo = rhythmRef.getTempo();
    if (rhythmTempo > kMinTempo) {
      rhythmRef.setTempo(rhythmTempo - 1);
      playerRef.calculateAndStartTimer();
      viewRef.drawTempo();
    }
  }

  void Controller::metronomeUp() {
    rhythmRef.setMetronome(!rhythmRef.getMetronome());
    viewRef.drawMetronome();
  }

  void Controller::metronomeDown() {
    rhythmRef.setMetronome(!rhythmRef.getMetronome());
    viewRef.drawMetronome();
  }

  void Controller::barUp() {
    uint8_t rhythmBar = rhythmRef.getBar();
    if (rhythmBar < kMaxBar) {
      playerRef.reset();
      viewRef.resetPlayBar();
      rhythmRef.setBar(rhythmBar + 1);
      adjustBarUpTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawBar();
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::barDown() {
    uint8_t rhythmBar = rhythmRef.getBar();
    if (rhythmBar > kMinBar) {
      playerRef.reset();
      viewRef.resetPlayBar();
      rhythmRef.setBar(rhythmBar - 1);
      adjustBarDownTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawBar();
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::measureUp() {
    uint8_t rhythmMeasure = rhythmRef.getMeasure();
    if (rhythmMeasure < kMaxMeasure) {
      playerRef.reset();
      viewRef.resetPlayBar();
      rhythmRef.setMeasure(rhythmMeasure + 1);
      adjustMeasureUpTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawMeasure();
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::measureDown() {
    uint8_t rhythmMeasure = rhythmRef.getMeasure();
    if (rhythmMeasure > kMinMeasure) {
      playerRef.reset();
      viewRef.resetPlayBar();
      rhythmRef.setMeasure(rhythmMeasure - 1);
      adjustMeasureDownTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawMeasure();
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::loadUp() {
    uint8_t load = rhythmRef.getLoad();
    if (load < kMaxLoad) {
      rhythmRef.setLoad(load + 1);
      viewRef.drawLoad();
    }
  }

  void Controller::loadDown() {
    uint8_t load = rhythmRef.getLoad();
    if (load > kMinLoad) {
      rhythmRef.setLoad(load - 1);
      viewRef.drawLoad();
    }
  }

  void Controller::saveUp() {
    uint8_t save = rhythmRef.getSave();
    if (save < kMaxSave) {
      rhythmRef.setSave(save + 1);
      viewRef.drawSave();
    }
  }

  void Controller::saveDown() {
    uint8_t save = rhythmRef.getSave();
    if (save > kMinSave) {
      rhythmRef.setSave(save - 1);
      viewRef.drawSave();
    }
  }

  void Controller::outputUp() {
    rhythmRef.setOutput(!rhythmRef.getOutput());
    viewRef.drawOutput();
  }

  void Controller::outputDown() {
    rhythmRef.setOutput(!rhythmRef.getOutput());
    viewRef.drawOutput();
  }

  void Controller::quantizeUp() {
    uint8_t rhythmQuantize = rhythmRef.getQuantize();
    if (rhythmQuantize < kMaxQuantize) {
      rhythmRef.setQuantize(rhythmQuantize + 1);
      viewRef.drawQuantize();
    }
  }

  void Controller::quantizeDown() {
    uint8_t rhythmQuantize = rhythmRef.getQuantize();
    if (rhythmQuantize > kMinQuantize) {
      rhythmRef.setQuantize(rhythmQuantize - 1);
      viewRef.drawQuantize();
    }
  }

  // layer menu functions

  void Controller::layerMenuRight() {
    if (currentLayerMenu < kMaxLayerMenu) {
      previousLayerMenu = currentLayerMenu;
      currentLayerMenu += 1;
      menuUpdate();
    }
  }

  void Controller::layerMenuLeft() {
    if (currentLayerMenu > 1) {
      previousLayerMenu = currentLayerMenu;
      currentLayerMenu -= 1;
      menuUpdate();
    }
  }

  void Controller::layerMenuUp() {
    switch (currentLayerMenu) {
      case 0:
      // statement
      break;
      case 1:
      instAUp();
      break;
      case 2:
      instBUp();
      break;
      default:
      break;
    }
  }

  void Controller::layerMenuDown() {
    switch (currentLayerMenu) {
      case 0:
      // statement
      break;
      case 1:
      instADown();
      break;
      case 2:
      instBDown();
      break;
      default:
      break;
    }
  }

  // layer functions

  void Controller::instAUp() {
    uint8_t layerInst = selectedLayer->getInstAMidi();
    if (layerInst < kMaxLayerInst) {
      selectedLayer->setInstAMidi(layerInst + 1);
      viewRef.drawInstA(*selectedLayer);
    }
  }

  void Controller::instADown() {
    uint8_t layerInst = selectedLayer->getInstAMidi();
    if (layerInst > kMinLayerInst) {
      selectedLayer->setInstAMidi(layerInst - 1);
      viewRef.drawInstA(*selectedLayer);
    }
  }

  void Controller::instBUp() {
    uint8_t layerInst = selectedLayer->getInstBMidi();
    if (layerInst < kMaxLayerInst) {
      selectedLayer->setInstBMidi(layerInst + 1);
      viewRef.drawInstB(*selectedLayer);
    }
  }

  void Controller::instBDown() {
    uint8_t layerInst = selectedLayer->getInstBMidi();
    if (layerInst > kMinLayerInst) {
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
      } else if (selectedLayer->getLastActiveBeat() != 0) {
        selectedBeatNum = 0;
      }
      selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      viewRef.drawFill(*selectedBeat);
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
    }
  }

  void Controller::beatDown() {
    // checking if layer is not empty
    if (selectedLayer->getLastActiveBeat() != -1) {
      if (selectedBeatNum > 0) {
        selectedBeatNum -= 1;
      } else if (selectedLayer->getLastActiveBeat() != 0) {
        selectedBeatNum = selectedLayer->getLastActiveBeat();
      }
      selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      viewRef.drawFill(*selectedBeat);
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
    }
  }

  void Controller::fillUp() {
    // checking if layer is not empty
    if (selectedLayer->getLastActiveBeat() != -1) {
      uint8_t beatFill = selectedBeat->getFill();
      if (beatFill < (kFillLibrarySize - 1)) {
        selectedLayer->setFill(selectedBeatNum, beatFill + 1);
        viewRef.drawFill(*selectedBeat);
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
        viewRef.drawFill(*selectedBeat);
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
      viewRef.drawFill(*selectedBeat);
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
      viewRef.drawFill(*selectedBeat);
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
        viewRef.cleanFill();
      } else {
        viewRef.drawFill(*selectedBeat);
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
