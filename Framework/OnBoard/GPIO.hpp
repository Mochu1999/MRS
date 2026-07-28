#pragma once

#include <gpiod.hpp>

//Currently only allows for output mode
struct GPIO
{
    //handle for the GPIOs 
    gpiod::line_request gpioHandle;

    GPIO(gpiod::line::offsets pins) //offsets meaning their controller lines positions
        : gpioHandle
        (
            gpiod::chip("/dev/gpiochip0") //The machine GPIO controller //Safe to assume it's 0 if we don't add more controllers
            .prepare_request() //Creates a gpiod::request_builder, an intermediate object storing the configuration for a line request
            //string identifying the software using the gpios lines for debugging purposes, configuration is associated to it
            .set_consumer("control").add_line_settings 
            (
                pins, //Applying the same settings to all pins
                //output signal (not input) and initially deactivated
                gpiod::line_settings().set_direction(gpiod::line::direction::OUTPUT).set_output_value(gpiod::line::value::INACTIVE)
            )
            .do_request() //Submits the configuration
        )
    {
    }

    void write(int pin, bool value)
    {
        gpioHandle.set_value(pin,
            value ? gpiod::line::value::ACTIVE : gpiod::line::value::INACTIVE);
    }
};