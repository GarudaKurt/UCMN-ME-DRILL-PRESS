#ifndef __LCDDISPLAY__H
#define __LCDDISPLAY__H
#include "motorControl.h"
extern MOTORCONTROL stepper;

#include <LiquidCrystal_I2C.h>

class LCDDISPLAY {
  public:
    LCDDISPLAY();
    ~LCDDISPLAY();
    void initLCD();
    void showHomeScreen();
    void showOptionA();
    void showOptionB();
    void showOptionC();
    void showInvalidMenu();
    void startCountdown(int durationMins);

  private:
    unsigned long prevMillis = 0;
    unsigned long remainingSeconds = 0;
    bool countdownActive = false;

    // Add this line
    int readOptionInput(char minKey, char maxKey);
};

#endif
