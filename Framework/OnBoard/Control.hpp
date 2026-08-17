#pragma once

#include "Parameters.hpp"
#include "GPIO.hpp"

#include <cmath>
#include <unistd.h>
#include <iostream>
#include <algorithm>

struct Control
{
    Parameters& p;

    //ids of the GPIOs
    int sailStepPin = 24;
    int sailDirectionPin = 23;

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
    }

        
    //void update()
    //{
    //    //updateSail();
    //    //updateRudder();

    //    gpio.write(sailStepPin, true);
    //    int pulse = 1500;
    //    usleep(pulse);


    //    gpio.write(sailStepPin, 0);
    //    usleep(20000-pulse);

    //}
    
    //void update()
    //{
    //    //updateSail();
    //    //updateRudder();
    //    int diff = 0;
    //    gpio.write(sailStepPin, true);
    //    int pulse = 1500+diff;

    //    usleep(pulse);


    //    gpio.write(sailStepPin, 0);
    //    usleep(20000-pulse);

    //    diff++;

    //}

    //void update()
    //{
    //    //updateSail();
    //    //updateRudder();
    //    int pulse = 100000;
    //    gpio.write(sailStepPin, 1);

    //    usleep(pulse);


    //    gpio.write(sailStepPin, 0);
    //    usleep(pulse);


    //}
    
    void update()
    {
        static int pulse = 1000;   // us
        static int dir = 1;

        gpio.write(sailStepPin, true);
        usleep(pulse);

        gpio.write(sailStepPin, false);
        usleep(20000 - pulse);

        pulse += dir * 10;

        if (pulse >= 2000) {
            pulse = 2000;
            dir = -1;
        }
        else if (pulse <= 1000) {
            pulse = 1000;
            dir = 1;
        }
        std::cout << "pulso es: " << pulse << " ms\n";

    }

    // For a servo HS-5646WP, -60º=900us, 0º=1500us, 60º=2100us
    //Receives angle in degrees, returns a pulse in us
    //int setAngleToPulse(float angle)
    //{
    //    
    //    if (angle < -60) angle = -60;
    //    if (angle > 60) angle = 60;

    //    return static_cast<int>(1500.0f + angle * 10.0f);
    //}

    //void updateRudder()
    //{
    //    if (p.rudderCounter > 0)
    //    {
    //        p.rudderCounter--;
    //        int pulse = setAngleToPulse(p.rudderAngle);

    //        gpio.write(sailStepPin, true);
    //        usleep(pulse);

    //        gpio.write(sailStepPin, false);
    //        usleep(20000-pulse);

    //        std::cout << "Moviendose a: " << p.rudderAngle << "º, pulso es: " << pulse << " ms\n";
    //    }
    //}

    //void updateSail()
    //{
    //    float difference = p.sailAngle - p.currentSailAngle;

    //    /*if (abs(difference) < sailAnglePerPulse)
    //        return;*/
    //    
    //    bool direction = (difference > 0.0f) ? 1 : 0;//1 CCW, 0 CW
    //        
    //    sendPulse(sailDirectionPin,sailStepPin,direction);

    //    p.currentSailAngle += (direction * sailAnglePerPulse);
    //}

    //void sendPulse(int gpioDirectionPin, int gpioStepPin, int direction)
    //{
    //    //Setting direction
    //    //gpioWrite(sailDirectionPin, direction);

    //    // STEP pulse
    //    gpio.write(sailStepPin, true);
    //    std::cout << "Light up\n";

    //    usleep(1000000); //creating the pulse by waiting //ASSUMPTION

    //    gpio.write(sailStepPin, 0);
    //    std::cout << "Light down\n";

    //}

};