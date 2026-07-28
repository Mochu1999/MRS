#pragma once
#include "Time.hpp"

struct Telemetry
{

	TimeStruct tm;

	//Ship variables
	p3 shipHeave = { 0,-0.056,0 };
	float shipHeaveIncrease = 0.0002;

	float rudderAngle = 0;
	float rudderIncrease = 0.2; //how much it changes with pressing the keyboard

	float sailAngle = 0;
	float sailIncrease = 0.4;

	p3 sailPosition;
	p3 modelSailPosition = { 0.602, 0.017, 0 };

	float battery = 1;

	Telemetry()
	{
		update();
	}

	void update()
	{
		tm.update();

		if (shipHeave.y >= -0.05 || shipHeave.y <= -0.1)
			shipHeaveIncrease = -shipHeaveIncrease;

		if (battery > 0)
			battery -= 0.0001;

		shipHeave.y += shipHeaveIncrease;

		sailPosition = modelSailPosition + shipHeave; //heave plus local position of sail

	}
};


