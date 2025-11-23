#include <Wire.h>
#include <Arduino.h>

#include "KeypadMenu.h"
#include "lcdDisplay.h"
#include "motorControl.h"

KEYPADMENU menu;
LCDDISPLAY display;
MOTORCONTROL control;

enum MenuState {
  HOME,
  OPTION_A,
  OPTION_B,
  OPTION_C,
  COUNTDOWN
};
MenuState currentState = HOME;
MenuState previousState = HOME;


const int btnForward = 45;
const int btnReverse = 46;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  pinMode(btnForward, INPUT_PULLUP);
  pinMode(btnReverse, INPUT_PULLUP);
  control.initStepper();
  display.initLCD();
  display.showHomeScreen();
}

void loop() {
  char key = menu.getInputs();

  if (key) {
    switch(currentState) {
      case HOME:
        if(key == 'A') currentState = OPTION_A;
        else if(key == 'B') currentState = OPTION_B;
        else if(key == 'C') currentState = OPTION_C;
        else display.showInvalidMenu();
        break;
      default: break;
    }
  }

  if(currentState != previousState) {
    switch(currentState) {
      case HOME: 
        menu.clearLastKey();
        display.showHomeScreen();
        break;

      case OPTION_A:
        display.showOptionA();
        currentState = HOME;
        break;

      case OPTION_B:
        display.showOptionB();
        currentState = HOME;
        break;

      case OPTION_C:
        display.showOptionC();
        currentState = HOME;
        break;
    }
    previousState = currentState;
  }

  if (digitalRead(btnForward) == LOW) {
    control.manualForwardStep();
  } else if (digitalRead(btnReverse) == LOW) {
    control.manualReverseStep();
  } else {
    control.stopMotor();
  }
}

