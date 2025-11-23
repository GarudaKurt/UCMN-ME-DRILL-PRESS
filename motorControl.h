#ifndef __MOTORCONTROL__H
#define __MOTORCONTROL__H

class MOTORCONTROL {
  public:
    MOTORCONTROL();
    ~MOTORCONTROL();
    void initStepper();
    void downWard();
    void upWard();
    void stopMotor();
    void manualForwardStep();
    void manualReverseStep();

  private:
};

#endif
