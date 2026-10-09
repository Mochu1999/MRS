


struct Parameters
{
  // --- --- ---
	// Configuration variables
	// --- --- ---

  //i2c
  int pinSDA = 16;
  int pinSCL = 17;
  
  //IMU
  uint8_t addressIMU = 0x68;

  //Servo
  int pinServo = 13;

  //Stepper
  int pinDir = 32;
  int pinStep = 33;

  //LoRa
  int pinLoRaRX = 22;
  int pinLoRaTX = 23;

  //GPS 
  int pinGPSRX = 34;


  // --- --- ---
	// Shared variables
	// --- --- ---
	float sailAngle = 0;
	float rudderAngle = 0;



  Parameters()
  {
    //setting common i2c
    Wire.begin(pinSDA, pinSCL);

  }
  
};