#pragma once


struct Parameters
{
	float sailAngle = 0;
	float currentSailAngle = 0;
	float rudderAngle = 0;
	float currentRudderAngle = 0;

	//Times it will send the rudder pulse after a new rudderAngle is given
	unsigned int rudderCounter = 10; //At the start it will send the pulse for 0º 
};