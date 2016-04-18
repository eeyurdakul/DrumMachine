#pragma once

#include <avr/pgmspace.h>
#include "Constant.h"

namespace Zebra {

  const PROGMEM uint8_t step00 = 1;
  const PROGMEM char name00[] = "Basic";
  const PROGMEM uint8_t time00[] = {0, 1};
  const PROGMEM uint8_t volume00[] = {3};
  const PROGMEM uint8_t inst00[] = {0};

  const PROGMEM uint8_t step01 = 2;
  const PROGMEM char name01[] = "Adolf";
  const PROGMEM uint8_t time01[] = {0, 1, 1};
  const PROGMEM uint8_t volume01[] = {3, 3};
  const PROGMEM uint8_t inst01[] = {0, 0};

  const PROGMEM uint8_t step02 = 3;
  const PROGMEM char name02[] = "Bryon";
  const PROGMEM uint8_t  time02[] = {0, 1, 1, 1};
  const PROGMEM uint8_t volume02[] = {3, 2, 1};
  const PROGMEM uint8_t inst02[] = {0, 0, 0};

  const PROGMEM uint8_t step03 = 4;
  const PROGMEM char name03[] = "Clark";
  const PROGMEM uint8_t  time03[] = {0, 1, 1, 1, 1};
  const PROGMEM uint8_t volume03[] = {3, 3, 3, 3};
  const PROGMEM uint8_t inst03[] = {0, 0, 0, 0};

  const PROGMEM uint8_t fillStepLibrary[] = {step00, step01, step02, step03};
  const PROGMEM char* const fillNameLibrary[] = {name00, name01, name02, name03};
  const PROGMEM uint8_t* const fillTimeLibrary[] = {time00, time01, time02, time03};
  const PROGMEM uint8_t* const fillVolumeLibrary[] = {volume00, volume01, volume02, volume03};
  const PROGMEM uint8_t* const fillInstLibrary[] = {inst00, inst01, inst02, inst03};
}
