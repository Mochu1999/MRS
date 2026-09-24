struct Parameters
{
  // --- --- ---
	// Configuration variables
	// --- --- ---
  int pinServo = 13;

  int pinDir = 32;
  int pinStep = 33;

  int pinLoRaRX = 15;
  int pinLoRaTX = 2;



  // --- --- ---
	// Shared variables
	// --- --- ---
	float sailAngle = 0;
	float rudderAngle = 0;



  Parameters()
  {
    pinMode(pinDir, OUTPUT);
    pinMode(pinStep, OUTPUT);
  }
  
};