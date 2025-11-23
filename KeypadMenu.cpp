#include "KeypadMenu.h"

KEYPADMENU::KEYPADMENU() {
  keypad = new Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
}

KEYPADMENU::~KEYPADMENU() {
  delete keypad;
}

char KEYPADMENU::getInputs() {
  return keypad->getKey(); // Use '->' for pointer
}

void KEYPADMENU::clearLastKey() {
  while (keypad->getKey()) {  }
}

