#include <ESP32Servo.h>
//Reductora stepper 1:5

struct Control
{
  Parameters& p;

  //The Servo type handles on its own the PWM logic, only the angle must be set
  Servo servo1;
  //Angles the ship is currently at. Gets compared with the ones in parameters and if there is a discrepancy the ship moves until they are reached
  float currentRudderAngle = 0; //Must be 0 at the start
  
  
  //Stepper
  float stepsPerRev = 200*16*5*70/90; //steps of the stepper by microstepping by reduction
  float stepsPerDegree = stepsPerRev/360; 
  // Assumes the sail is at 0º (equivalent to 0 pulses) at the start
  int currentSailStep = 0; 


  Control(Parameters& p_)
    : p(p_)
  {
    servo1.attach(p.pinServo);
    servo1.write(90); //forcing it to start at 0
  }

  void update()
  {
    updateRudder();
    updateSail();
  }


  void updateRudder()
  {
    if (currentRudderAngle != p.rudderAngle)
    {
      //our 0º is the 90º from the library
      servo1.write(p.rudderAngle + 90);
      currentRudderAngle = p.rudderAngle;
    }
  }

  void updateSail()
  {
    int targetStep = round(p.sailAngle * stepsPerDegree);

    if (targetStep != currentSailStep)
    {
        int stepsDifference = targetStep - currentSailStep;

        // 1 CCW, 0 CW
        bool direction = (stepsDifference > 0) ? 1 : 0;
        digitalWrite(p.pinDir, direction);

        while (currentSailStep != targetStep)
        {
            digitalWrite(p.pinStep, HIGH);
            delayMicroseconds(1000);

            digitalWrite(p.pinStep, LOW);
            delayMicroseconds(1000);

            if (direction) currentSailStep++;
            else currentSailStep--;
        }
    }
  }
};


