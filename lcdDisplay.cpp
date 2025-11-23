#include "lcdDisplay.h"
#include "motorControl.h"
#include <Arduino.h>
#include "KeypadMenu.h"

MOTORCONTROL stepper;
LiquidCrystal_I2C lcd(0x27, 20, 4);
extern KEYPADMENU menu;
const int buz = 13;

LCDDISPLAY::LCDDISPLAY() {}
LCDDISPLAY::~LCDDISPLAY() {}

void LCDDISPLAY::initLCD() {
  pinMode(buz, OUTPUT);
  noTone(buz);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(3, 2);
  lcd.print("System ready..");
  delay(2000);
}

void LCDDISPLAY::showHomeScreen() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("| AUTO DRILL PRESS |");
  lcd.setCursor(2,1);
  lcd.print("[A] for 5 mins");
  lcd.setCursor(2,2);
  lcd.print("[B] for 10 mins");
  lcd.setCursor(2,3);
  lcd.print("[C] for 15 mins");
}

void LCDDISPLAY::showOptionA() {
  menu.clearLastKey();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("| SELECT OPTION A |");
  lcd.setCursor(2,1);
  lcd.print("[1] for 1 min");
  lcd.setCursor(2,2);
  lcd.print("[2] for 3 mins");
  lcd.setCursor(2,3);
  lcd.print("[3] for 5 mins");

  int choice = readOptionInput('1', '3');

  if(choice == 1) startCountdown(1);
  else if(choice == 2) startCountdown(3);
  else if(choice == 3) startCountdown(5);
}

void LCDDISPLAY::showOptionB() {
  menu.clearLastKey();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("| SELECT OPTION B |");
  lcd.setCursor(2,1);
  lcd.print("[1] for 6 mins");
  lcd.setCursor(2,2);
  lcd.print("[2] for 8 mins");
  lcd.setCursor(2,3);
  lcd.print("[3] for 10 mins");

  int choice = readOptionInput('1', '3');

  if(choice == 1) startCountdown(6);
  else if(choice == 2) startCountdown(8);
  else if(choice == 3) startCountdown(10);
}

void LCDDISPLAY::showOptionC() {
  menu.clearLastKey();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("| SELECT OPTION C |");
  lcd.setCursor(2,1);
  lcd.print("[1] for 11 mins");
  lcd.setCursor(2,2);
  lcd.print("[2] for 13 mins");
  lcd.setCursor(2,3);
  lcd.print("[3] for 15 mins");

  int choice = readOptionInput('1', '3');

  if(choice == 1) startCountdown(11);
  else if(choice == 2) startCountdown(13);
  else if(choice == 3) startCountdown(15);
}

void LCDDISPLAY::showInvalidMenu() {
  lcd.clear();
  lcd.setCursor(2,2);
  lcd.print("Invalid menu!");
  delay(2000);
  showHomeScreen();
}

int LCDDISPLAY::readOptionInput(char minKey, char maxKey) {
  while(true) {
    char key = menu.getInputs();
    if(key >= minKey && key <= maxKey) {
      return key - '0';
    }
    else if(key == 'D') {
      menu.clearLastKey();
      lcd.clear();
      lcd.setCursor(4,1);
      lcd.print("Cancelled!");
      lcd.setCursor(2,3);
      lcd.print("Returning...");
      noTone(buz);
      delay(2000);
      stepper.upWard();
      return 0;
    }
  }
}

void LCDDISPLAY::startCountdown(int durationMins) {
  remainingSeconds = durationMins * 60;
  countdownActive = true;
  prevMillis = millis();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Press [D] to cancel");
  lcd.setCursor(0,1);
  lcd.print("Press [#] to Abort");
  lcd.setCursor(3,2);
  lcd.print("Remaining Time:");

  while(countdownActive) {
    unsigned long currentMillis = millis();
    if(currentMillis - prevMillis >= 1000) {
      prevMillis = currentMillis;
      if(remainingSeconds > 0) {
        stepper.downWard();
        remainingSeconds--;
        int min = remainingSeconds / 60;
        int sec = remainingSeconds % 60;
        lcd.setCursor(5,3);
        lcd.print("     ");
        lcd.setCursor(5,3);
        if(min<10) lcd.print("0");
        lcd.print(min);
        lcd.print(":");
        if(sec<10) lcd.print("0");
        lcd.print(sec);
      } else {
        unsigned long upStart = millis();
        unsigned long lastUpStep = millis();
        int upCounter = 30;

        while (upCounter >= 0) {

          stepper.upWard();
          unsigned long now = millis();
          if (now - lastUpStep >= 1000) {
              lastUpStep = now;
              upCounter--;
          }
          lcd.clear();
          lcd.setCursor(4,1);
          lcd.print("Timer Done!");
          lcd.setCursor(8, 2);
          lcd.print(" ");
          lcd.setCursor(8, 2);
          lcd.print(upCounter);
          char key = menu.getInputs();
          if (key == 'D') break;
          else if(key == '#') break;
          menu.clearLastKey();
        }


        countdownActive = false;
        lcd.clear();
        lcd.setCursor(4,1);
        lcd.print("Timer Done!");
        lcd.setCursor(3,3);
        lcd.print("Beep...Beep...");

        for(int i=0;i<10;i++){
          tone(buz, 494);
          delay(1000);
        }
        noTone(buz);
        lcd.clear();
        lcd.setCursor(2,2);
        lcd.print("Returning...");
        delay(1500);
        showHomeScreen();
        menu.clearLastKey();
      }
    }
    char key = menu.getInputs();
    if(key == 'D') {
      countdownActive = false;
      lcd.clear();
      lcd.setCursor(4,1);
      lcd.print("Cancelled!");
      lcd.setCursor(2,3);
      lcd.print("Returning...");
      noTone(buz);
      delay(2000);
      stepper.upWard();

      unsigned long upStart = millis();
      unsigned long lastUpStep = millis();
      int upCounter = 30;

      while (upCounter < 0) {
        stepper.upWard();      
        unsigned long now = millis();
        if (now - lastUpStep >= 1000) {
          lastUpStep = now;
          upCounter--;  
        }
        lcd.clear();
        lcd.setCursor(2, 1);
        lcd.print("Press [#] to Abort");
        lcd.setCursor(4,2);
        lcd.print("Timer Done!");
        lcd.setCursor(8, 3);
        lcd.print(" ");
        lcd.setCursor(8, 3);
        lcd.print(upCounter);
      }
      showHomeScreen();
    } else if(key == '#') {
      Serial.println("HERE 1");
      stepper.stopMotor();
      lcd.clear();
      lcd.setCursor(3, 2);
      lcd.print("Emergency Stop!");
      delay(3000);
      showHomeScreen();
      return;
    }
  }
}
