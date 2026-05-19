#pragma once
#include "Time.hpp"

struct Telemetry
{

	TimeStruct tm;

	//Ship variables
	p3 shipHeave = { 0,-0.056,0 };
	float shipHeaveIncrease = 0.0002;

	float rudderAngle = 0;
	float rudderIncrease = 0.2;

	float sailAngle = 0;
	float sailIncrease = 0.4;
	p3 sailPosition;

	float battery = 1;

	Telemetry()
	{
		update();
	}

	void update()
	{
		tm.update();

		/*if (rudderAngle >= 20 || rudderAngle <= -20)
			rudderIncrease = -rudderIncrease;
		if (sailAngle >= 60 || sailAngle <= -60)
			sailIncrease = -sailIncrease;*/
		if (shipHeave.y >= -0.05 || shipHeave.y <= -0.1)
			shipHeaveIncrease = -shipHeaveIncrease;

		if (battery > 0)
			battery -= 0.0001;
		/*rudderAngle += rudderIncrease;
		sailAngle += sailIncrease;*/
		shipHeave.y += shipHeaveIncrease;

		sailPosition = shipHeave + p3{ 0.602, 0.017, 0 }; //heave plus local position of sail

	}
};


