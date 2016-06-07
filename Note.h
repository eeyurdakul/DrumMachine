[*] Timeframe class
[*] Timeline class
[*] Beat class
[*] Layer class
[*] Rhythm class
[*] Player class
[*] Clear info area effectively
[-] Layer Inst selection

[*] Add Beat Button
[*] BeatClear Button

[*] View.drawSelectedBeat
[*] View.drawBeatFill

[*] add if (&layer & &beat != NULL) protection to view classes

[-] add number variable into beat class (optional)

[-] Quantize Calculation
[-] Midi-out function

playPeriod = 60.000.000 / (tempo * 64)
playFrequency = 1.000.000 / beatTime





void View::drawMenuBox(uint16_t boxNum, bool state) {
  uint16_t backColor;
  uint16_t foreColor;
  uint16_t boxXPos;
  uint16_t boxYPos;
  uint16_t headerXPos;
  uint16_t headerYPos;
  uint8_t dataDigit;
  uint8_t dataXPos;
  uint8_t dataYPos;
  // defining variables
  menuColorSelect(backColor, foreColor, state);
  // taking actions
  fillRect(boxXPos, boxYPos, kMenuBoxWidth, kMenuBoxHeight, backColor);
  setTextColor(foreColor);
  setCursor(headerXPos, headerYPos);
  println(header);
  drawInfoNumber(data, digit, dataXPos, dataYPos, foreColor);
}
