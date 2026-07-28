#pragma once

#include "Parameters.hpp"
#include "GPIO.hpp"

#include <cmath>
#include <unistd.h>

struct Control
{
    Parameters& p;

    //ids of the GPIOs
    int sailStepPin = 23;
    int sailDirectionPin = 24;

    GPIO gpio;

    int motorStepsPerRevolution = 200; //ASSUMPTION
    int microsteps = 16; //ASSUMPTION
    float gearRatio = 1.0f; //ASSUMPTION

    float sailAnglePerPulse;
    float rudderAnglePerPulse;

    Control(Parameters& p_)
        : p(p_)
        , gpio({sailStepPin, sailDirectionPin})
    {
        sailAnglePerPulse = 360.0f / (motorStepsPerRevolution * microsteps * gearRatio);

        gpio.write(sailStepPin, 0);

    }

    void update()
    {
        //updateSail();
        //updateRudder();

        gpio.write(sailStepPin, true);
        usleep(100000);


        gpio.write(sailStepPin, 0);
        usleep(100000);

    }

    void updateSail()
    {
        float difference = p.sailAngle - p.currentSailAngle;

        /*if (abs(difference) < sailAnglePerPulse)
            return;*/
        
        bool direction = (difference > 0.0f) ? 1 : 0;//1 CCW, 0 CW
            
        sendPulse(sailDirectionPin,sailStepPin,direction);

        p.currentSailAngle += (direction * sailAnglePerPulse);
    }

    void sendPulse(int gpioDirectionPin, int gpioStepPin, int direction)
    {
        //Setting direction
        //gpioWrite(sailDirectionPin, direction);

        // STEP pulse
        gpio.write(sailStepPin, true);
        std::cout << "Light up\n";

        usleep(1000000); //creating the pulse by waiting //ASSUMPTION

        gpio.write(sailStepPin, 0);
        std::cout << "Light down\n";

    }

    //void updateRudder()
    //{
    //    float difference = p.sailAngle - p.currentRudderAngle;

    //    if (abs(difference) < rudderAnglePerPulse)
    //        return;

    //    bool direction = (difference > 0.0f) ? 1 : 0;//1 CCW, 0 CW

    //    //sendPulse(rudderDirectionPin, rudderStepPin, direction);

    //    p.currentRudderAngle += (direction * rudderAnglePerPulse);
    //}
};