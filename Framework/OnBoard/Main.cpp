

//#include "Parameters.hpp"
//#include "Control.hpp"
//
//#include "LoRa.hpp"

#include "GPIO.hpp"

//It constantly tries to read the serial to update its variables. 
// If waitingSendBackTime has elapsed it sends the message back to the windows for confirmation that it has received the last parameters
// If the confirmation does not happen or the values have been updated, the pc will send the message back until correct confirmation is given

int main()
{
	/*Parameters p;
	Control control(p);
	LoRa lora(p);*/

	int sailStepPin = 24;
	GPIO gpio({ sailStepPin });
	std::cout << "a\n";



	while (true)
	{
		//lora.update();
		//control.update();

		//usleep(10000); // 10 ms

		/*int pulse = 1800;

		gpio.write(sailStepPin, true);
		usleep(pulse);

		gpio.write(sailStepPin, false);
		usleep(20000-pulse);*/
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


	return 0;
}