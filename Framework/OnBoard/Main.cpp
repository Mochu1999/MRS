

#include "Parameters.hpp"
#include "Control.hpp"

#include "LoRa.hpp"

//It constantly tries to read the serial to update its variables. 
// If waitingSendBackTime has elapsed it sends the message back to the windows for confirmation that it has received the last parameters
// If the confirmation does not happen or the values have been updated, the pc will send the message back until correct confirmation is given

int main()
{
    Parameters p;
    Control control(p);
    LoRa lora(p);

    while (true)
    {
        lora.update();
        control.update();

        //usleep(10000); // 10 ms
    }

    
    return 0;
}