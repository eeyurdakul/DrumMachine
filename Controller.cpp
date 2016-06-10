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
  , currentRhythmMenu(0)
  , previousRhythmMenu(-1)
  , currentLayerMenu(-1)
  , previousLayerMenu(-1) {}

  Controller::~Controller() {}

  void Controller::initialize() {
    rhythmMenuUpdate();
    viewRef.resetPlayBar();
    viewRef.drawAllLayer();
  }

  void Controller::checkKeyboard() {
    // select buttons
    if (keyboard.rhythmMenuSelectButton.checkStatus()) {
      rhythmMenuSelect();
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
      if (currentRhythmMenu >= 0) {
        rhythmMenuUp();
      } else {
        layerMenuUp();
      }
    }
    if (keyboard.downButton.checkStatus()) {
      if (currentRhythmMenu >= 0) {
        rhythmMenuDown();
      } else {
        layerMenuDown();
      }
    }
    if (keyboard.rightButton.checkStatus()) {
      if (currentRhythmMenu >= 0) {
        rhythmMenuRight();
      } else {
        layerMenuRight();
      }
    }
    if (keyboard.leftButton.checkStatus()) {
      if (currentRhythmMenu >= 0) {
        rhythmMenuLeft();
      } else {
        layerMenuLeft();
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

  void Controller::rhythmMenuSelect() {
    if (currentRhythmMenu == -1) {
      for (uint8_t i = 0; i < kLayerLibrarySize; i++) {
        rhythmRef.getLayer(i).setSelected(false);
        viewRef.drawLayerSelected(rhythmRef.getLayer(i));
      }
      // rhythm menu settings
      currentRhythmMenu = 0;
      previousRhythmMenu = -1;
      // layer menu settings
      currentLayerMenu = -1;
      previousLayerMenu = -1;
      selectedLayer = NULL;
      selectedBeat = NULL;
      selectedBeatNum = -1;
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      rhythmMenuUpdate();
    }
  }

  void Controller::layerSelect(Layer& layer_) {
    if (selectedLayer == NULL) {
      // rhythm menu settings
      currentRhythmMenu = -1;
      previousRhythmMenu = -1;
      // layer menu settings
      currentLayerMenu = 0;
      previousLayerMenu = -1;
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
      layerMenuUpdate();
    } else if (layer_.getNumber() != selectedLayer->getNumber()) {
      // layer menu settings
      currentLayerMenu = 0;
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
      layerMenuUpdate();
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

  // rhythm menu functions

  void Controller::rhythmMenuRight() {
    previousRhythmMenu = currentRhythmMenu;
    if (currentRhythmMenu < kMaxRhythmMenu) {
      currentRhythmMenu += 1;
      rhythmMenuUpdate();
    }
  }

  void Controller::rhythmMenuLeft() {
    previousRhythmMenu = currentRhythmMenu;
    if (currentRhythmMenu > 0) {
      currentRhythmMenu -= 1;
      rhythmMenuUpdate();
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

  void Controller::rhythmMenuUpdate() {
    switch (currentRhythmMenu) {
      case 0:   // tempo
      if (previousRhythmMenu == -1) {
        viewRef.drawRhythmMenuBox(kTempoBox, 1);
        viewRef.drawRhythmMenuBox(kMetronomeBox, 0);
        viewRef.drawRhythmMenuBox(kBarBox, 0);
        viewRef.drawRhythmMenuBox(kMeasureBox, 0);
      } else if (previousRhythmMenu == 1) {
        viewRef.drawRhythmMenuBox(kTempoBox, 1);
        viewRef.drawRhythmMenuBox(kMetronomeBox, 0);
      }
      break;
      case 1:   // metronome
      if (previousRhythmMenu == 0) {
        viewRef.drawRhythmMenuBox(kTempoBox, 0);
        viewRef.drawRhythmMenuBox(kMetronomeBox, 1);
      } else if (previousRhythmMenu == 2) {
        viewRef.drawRhythmMenuBox(kMetronomeBox, 1);
        viewRef.drawRhythmMenuBox(kBarBox, 0);
      }
      break;
      case 2:   // bar
      if (previousRhythmMenu == 1) {
        viewRef.drawRhythmMenuBox(kMetronomeBox, 0);
        viewRef.drawRhythmMenuBox(kBarBox, 1);
      } else if (previousRhythmMenu == 3) {
        viewRef.drawRhythmMenuBox(kBarBox, 1);
        viewRef.drawRhythmMenuBox(kMeasureBox, 0);
      }
      break;
      case 3:   // measure
      if (previousRhythmMenu == 2) {
        viewRef.drawRhythmMenuBox(kBarBox, 0);
        viewRef.drawRhythmMenuBox(kMeasureBox, 1);
      } else if (previousRhythmMenu == 4) {
        viewRef.drawRhythmMenuBox(kTempoBox, 0);
        viewRef.drawRhythmMenuBox(kMetronomeBox, 0);
        viewRef.drawRhythmMenuBox(kBarBox, 0);
        viewRef.drawRhythmMenuBox(kMeasureBox, 1);
      }
      break;
      case 4:   // load
      if (previousRhythmMenu == 3) {
        viewRef.drawRhythmMenuBox(kLoadBox, 1);
        viewRef.drawRhythmMenuBox(kSaveBox, 0);
        viewRef.drawRhythmMenuBox(kOutputBox, 0);
        viewRef.drawRhythmMenuBox(kQuantizeBox, 0);
      } else if (previousRhythmMenu == 5) {
        viewRef.drawRhythmMenuBox(kLoadBox, 1);
        viewRef.drawRhythmMenuBox(kSaveBox, 0);
      }
      break;
      case 5:   // save
      if (previousRhythmMenu == 4) {
        viewRef.drawRhythmMenuBox(kLoadBox, 0);
        viewRef.drawRhythmMenuBox(kSaveBox, 1);
      } else if (previousRhythmMenu == 6) {
        viewRef.drawRhythmMenuBox(kSaveBox, 1);
        viewRef.drawRhythmMenuBox(kOutputBox, 0);
      }
      break;
      case 6:   // output
      if (previousRhythmMenu == 5) {
        viewRef.drawRhythmMenuBox(kSaveBox, 0);
        viewRef.drawRhythmMenuBox(kOutputBox, 1);
      } else if (previousRhythmMenu == 7) {
        viewRef.drawRhythmMenuBox(kOutputBox, 1);
        viewRef.drawRhythmMenuBox(kQuantizeBox, 0);
      }
      break;
      case 7:   // quantize
      if (previousRhythmMenu == 6) {
        viewRef.drawRhythmMenuBox(kOutputBox, 0);
        viewRef.drawRhythmMenuBox(kQuantizeBox, 1);
      }
      break;
      default:
      break;
    }
  }

  // rhythm functions

  void Controller::tempoUp() {
    uint8_t tempo = rhythmRef.getTempo();
    if (tempo < kMaxTempo) {
      rhythmRef.setTempo(tempo + 1);
      playerRef.calculateAndStartTimer();
      viewRef.drawRhythmMenuData(kTempoBox);
    }
  }

  void Controller::tempoDown() {
    uint8_t tempo = rhythmRef.getTempo();
    if (tempo > kMinTempo) {
      rhythmRef.setTempo(tempo - 1);
      playerRef.calculateAndStartTimer();
      viewRef.drawRhythmMenuData(kTempoBox);
    }
  }

  void Controller::metronomeUp() {
    rhythmRef.setMetronome(!rhythmRef.getMetronome());
    viewRef.drawRhythmMenuData(kMetronomeBox);
  }

  void Controller::metronomeDown() {
    rhythmRef.setMetronome(!rhythmRef.getMetronome());
    viewRef.drawRhythmMenuData(kMetronomeBox);
  }

  void Controller::barUp() {
    uint8_t bar = rhythmRef.getBar();
    if (bar < kMaxBar) {
      playerRef.reset();
      rhythmRef.setBar(bar + 1);
      adjustBarUpTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawRhythmMenuData(kBarBox);
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::barDown() {
    uint8_t bar = rhythmRef.getBar();
    if (bar > kMinBar) {
      playerRef.reset();
      rhythmRef.setBar(bar - 1);
      adjustBarDownTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawRhythmMenuData(kBarBox);
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::measureUp() {
    uint8_t measure = rhythmRef.getMeasure();
    if (measure < kMaxMeasure) {
      playerRef.reset();
      rhythmRef.setMeasure(measure + 1);
      adjustMeasureUpTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawRhythmMenuData(kMeasureBox);
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::measureDown() {
    uint8_t measure = rhythmRef.getMeasure();
    if (measure > kMinMeasure) {
      playerRef.reset();
      rhythmRef.setMeasure(measure - 1);
      adjustMeasureDownTiming();
      viewRef.calculatePlayXRatio();
      viewRef.drawRhythmMenuData(kMeasureBox);
      viewRef.drawAllLayerMeasureSongFill();
    }
  }

  void Controller::loadUp() {
    uint8_t load = rhythmRef.getLoad();
    if (load < kMaxLoad) {
      rhythmRef.setLoad(load + 1);
      viewRef.drawRhythmMenuData(kLoadBox);
    }
  }

  void Controller::loadDown() {
    uint8_t load = rhythmRef.getLoad();
    if (load > kMinLoad) {
      rhythmRef.setLoad(load - 1);
      viewRef.drawRhythmMenuData(kLoadBox);
    }
  }

  void Controller::saveUp() {
    uint8_t save = rhythmRef.getSave();
    if (save < kMaxSave) {
      rhythmRef.setSave(save + 1);
      viewRef.drawRhythmMenuData(kSaveBox);
    }
  }

  void Controller::saveDown() {
    uint8_t save = rhythmRef.getSave();
    if (save > kMinSave) {
      rhythmRef.setSave(save - 1);
      viewRef.drawRhythmMenuData(kSaveBox);
    }
  }

  void Controller::outputUp() {
    rhythmRef.setOutput(!rhythmRef.getOutput());
    viewRef.drawRhythmMenuData(kOutputBox);
  }

  void Controller::outputDown() {
    rhythmRef.setOutput(!rhythmRef.getOutput());
    viewRef.drawRhythmMenuData(kOutputBox);
  }

  void Controller::quantizeUp() {
    uint8_t quantize = rhythmRef.getQuantize();
    if (quantize < kMaxQuantize) {
      rhythmRef.setQuantize(quantize + 1);
      viewRef.drawRhythmMenuData(kQuantizeBox);
    }
  }

  void Controller::quantizeDown() {
    uint8_t quantize = rhythmRef.getQuantize();
    if (quantize > kMinQuantize) {
      rhythmRef.setQuantize(quantize - 1);
      viewRef.drawRhythmMenuData(kQuantizeBox);
    }
  }

  // layer menu functions

  void Controller::layerMenuRight() {
    previousLayerMenu = currentLayerMenu;
    if (currentLayerMenu < kMaxLayerMenu) {
      currentLayerMenu += 1;
      layerMenuUpdate();
    }
  }

  void Controller::layerMenuLeft() {
    previousLayerMenu = currentLayerMenu;
    if (currentLayerMenu > 0) {
      currentLayerMenu -= 1;
      layerMenuUpdate();
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

  void Controller::layerMenuUpdate() {
    switch (currentLayerMenu) {
      case 0:   // fill
      if (previousLayerMenu == -1) {
        viewRef.drawLayerMenuBox(kFillBox, 1, *selectedLayer);
        viewRef.drawLayerMenuBox(kInstABox, 0, *selectedLayer);
        viewRef.drawLayerMenuBox(kInstBBox, 0, *selectedLayer);
      } else if (previousLayerMenu == 1) {
        viewRef.drawLayerMenuBox(kFillBox, 1, *selectedLayer);
        viewRef.drawLayerMenuBox(kInstABox, 0, *selectedLayer);
      }
      break;
      case 1:   // inst A
      if (previousLayerMenu == 0) {
        viewRef.drawLayerMenuBox(kFillBox, 0, *selectedLayer);
        viewRef.drawLayerMenuBox(kInstABox, 1, *selectedLayer);
      } else if (previousLayerMenu == 2) {
        viewRef.drawLayerMenuBox(kInstABox, 1, *selectedLayer);
        viewRef.drawLayerMenuBox(kInstBBox, 0, *selectedLayer);
      }
      break;
      case 2:   // inst B
      if (previousLayerMenu == 1) {
        viewRef.drawLayerMenuBox(kInstABox, 0, *selectedLayer);
        viewRef.drawLayerMenuBox(kInstBBox, 1, *selectedLayer);
      }
      break;
    }
  }

  // layer functions

  void Controller::beatUp() {
    // checking if layer is not empty
    if (selectedLayer->getLastActiveBeat() != -1) {
      if (selectedBeatNum < selectedLayer->getLastActiveBeat()) {
        selectedBeatNum += 1;
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
        // viewRef.drawInfoFill(*selectedBeat);
        viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      } else if (selectedLayer->getLastActiveBeat() != 0) {
        selectedBeatNum = 0;
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
        // viewRef.drawInfoFill(*selectedBeat);
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
        // viewRef.drawInfoFill(*selectedBeat);
        viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      } else if (selectedLayer->getLastActiveBeat() != 0) {
        selectedBeatNum = selectedLayer->getLastActiveBeat();
        selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
        // viewRef.drawInfoFill(*selectedBeat);
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
        // viewRef.drawInfoFill(*selectedBeat);
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
        // viewRef.drawInfoFill(*selectedBeat);
        if (selectedBeat->getFill() == 0) {
          viewRef.drawBeatFill(*selectedLayer, selectedBeatNum, false);
        }
      }
    }
  }

  void Controller::instAUp() {
    uint8_t layerInst = selectedLayer->getInstAMidi();
    if (layerInst < kMaxLayerInstMidi) {
      selectedLayer->setInstAMidi(layerInst + 1);
      // viewRef.drawInstA(*selectedLayer);
    }
  }

  void Controller::instADown() {
    uint8_t layerInst = selectedLayer->getInstAMidi();
    if (layerInst > kMinLayerInstMidi) {
      selectedLayer->setInstAMidi(layerInst - 1);
      // viewRef.drawInstA(*selectedLayer);
    }
  }

  void Controller::instBUp() {
    uint8_t layerInst = selectedLayer->getInstBMidi();
    if (layerInst < kMaxLayerInstMidi) {
      selectedLayer->setInstBMidi(layerInst + 1);
      // viewRef.drawInstB(*selectedLayer);
    }
  }

  void Controller::instBDown() {
    uint8_t layerInst = selectedLayer->getInstBMidi();
    if (layerInst > kMinLayerInstMidi) {
      selectedLayer->setInstBMidi(layerInst - 1);
      //viewRef.drawInstB(*selectedLayer);
    }
  }

  // beat record functions

  void Controller::beatA() {
    if (playerRef.getRecordActive() && (selectedLayer != NULL)) {
      uint32_t recordTime = rhythmRef.getPlayTime();
      bool recordInst = 0;
      selectedBeatNum = selectedLayer->setBeat(recordTime, 3, recordInst);
      selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      viewRef.drawLayerBeat(*selectedLayer, recordTime, recordInst);
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      // viewRef.drawInfoFill(*selectedBeat);
    }
  }

  void Controller::beatB() {
    if (playerRef.getRecordActive() && (selectedLayer != NULL)) {
      uint32_t recordTime = rhythmRef.getPlayTime();
      bool recordInst = 1;
      selectedBeatNum = selectedLayer->setBeat(recordTime, 3, recordInst);
      selectedBeat = &(selectedLayer->getBeat(selectedBeatNum));
      viewRef.drawLayerBeat(*selectedLayer, recordTime, recordInst);
      viewRef.drawSelectedBeat(*selectedLayer, *selectedBeat);
      // viewRef.drawInfoFill(*selectedBeat);
    }
  }

  void Controller::beatClear() {
    if (selectedLayer != NULL) {
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
        // viewRef.cleanInfoFill();
      } else {
        // viewRef.drawInfoFill(*selectedBeat);
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
