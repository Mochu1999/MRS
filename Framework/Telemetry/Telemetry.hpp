#pragma once
#include "Time.hpp"
#include "AuxLonLats.hpp"

struct Telemetry
{
	TimeStruct tm;

	p2 position = { 2.128842,41.248926 }; //in LonLats
	p2 finishPoint = { 1.25,39.05 };

	float greatCircleDistance; //spherical distance in meters
	float totalDistance; //Distance following the nodes

	float headingAngle = 0; //Where the bow is pointing //In dregrees, where north 0º, east 90º...

	float rudderAngle = 0;
	float rudderIncrease = 0.2; //how much it changes by pressing the keyboard

	float sailAngle = 0;
	float sailIncrease = 0.4; //how much it changes by pressing the keyboard

	float battery = 1; //Percentage of battery

	float speed = 0; //m/s


	p3 sailPosition = { 0.602, 0.017, 0 }; //Coordinates where the sail model should be (otherwise is centered on 0)

	//Visual heave effect
	p3 shipHeave = { 0,-0.056,0 };
	float shipHeaveIncrease = 0.0002;


	Telemetry()
	{
		update();
	}

	void update()
	{
		tm.update();

		updatePosition();

		//Visual heave effect
		if (shipHeave.y >= -0.05 || shipHeave.y <= -0.1)
			shipHeaveIncrease = -shipHeaveIncrease;
		shipHeave.y += shipHeaveIncrease;

	}

	void updatePosition()
	{
		greatCircleDistance = calculateDistance(finishPoint, position);

		//USAR CON NODOS
		totalDistance = greatCircleDistance;
	}
};


