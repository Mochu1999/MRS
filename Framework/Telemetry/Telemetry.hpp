#pragma once
#include "Time.hpp"
#include "AuxLonLats.hpp"


//SEPARAR TELEMETRÍA CON PARÁMETROS INHERENTES AL BARCO
struct Telemetry
{
	TimeStruct tm;

	//all angles in degrees, the rest in IS
	
	//north 0º, east will be shown as 90º, but as it is cw, internally it will be shown as -90 to respect the positive=ccw standard and so on
	float headingAngle = 0; //Where the bow is pointing

	p2 shipSpeedLocal; //{surge,sway} with respect of the headingAngle //m/s
	p2 shipSpeed; // speed in global axis

	float driftAngle = 0; //0º is there's no sway


	p2 trueWind; //global axis
	float trueWindSpeed = 0; //m/s
	float trueWindAngle = 0; //º around north

	p2 appWind; //apparent wind
	p2 appWindDir; //unitary value of appWind

	float sailAngleLocal = 0; //local coordinates
	float sailAngle = 0; //global axis

	float sailAngleOfAttack = 0; // Difference between sailAngle and appWindAngle


	float rudderAngleLocal = 0; 
	float rudderAngle = 0;



	//Valores barco 3 metros rezola
	float L = 3;
	float B = 0.3; //one hull
	p3 sailPos = { 1.5,1.5,0 };
	p3 keelPos = { 1.5,-0.3,0 };
	p3 rudderPos = { 0.1,-0.15,0 };
	p3 CG = { 1.3,0.5,0 };

	void calculateVariables()
	{
		// --- --- ---
		// True wind to global axes
		// --- --- ---
		//We turn trueWindSpeed and trueWindAngle into a single p2 with +x being wind pointing north and +y wind pointing west
		// First we make it negative because it points towards where it's coming. if the angle is 0º it's going to -x south, not to the +x north
		//We make a generic vector and then we rotate it
		trueWind = { -trueWindSpeed,0 };
		//Our angle convention goes the other way, 90º cw (east), not ccw (would have been west)
		rotateP2(trueWind, -trueWindAngle);


		// --- --- ---
		// Ship's speed to global axes
		// --- --- ---
		//we calculate the global one from the know local one
		shipSpeed = shipSpeedLocal;
		rotateP2(shipSpeed, -headingAngle);

		driftAngle = degrees(atan2(shipSpeedLocal.y, shipSpeedLocal.x));


		// --- --- ---
		// Apparent wind 
		// --- --- ---
		appWind = trueWind - shipSpeed;

		float appWindAngle = degrees(atan2(appWind.y, appWind.x));
		//float appWindSpeed = magnitude2(appWind);

		appWindDir = normalize2(appWind);
		//Solo se usa una vez en cálculo de fuerzas, ponerlo directamente ahí
		//p2 appWindDirPerpendicular = { appWindDir.y,-appWindDir.x };


		// --- --- ---
		// Sail Angle of Attack
		// --- --- ---
		// Putting the angle of the sail that we had on global coordinates
		// The negative to follow the angles of the rose, the 180 to flip it so the vector points backwards along the sail (Rezola's AoA sign convention)
		sailAngle = 180.0f - (headingAngle + sailAngleLocal);

		// Difference between sailAngle and appWindAngle
		// remainder takes any angle and rewrites it as the equivalent signed angle closest to zero, so range is -180,180 (the closest)
		sailAngleOfAttack = std::remainder(sailAngle - appWindAngle, 360.0f);


		// --- --- ---
		// Keel Angle of Attack
		// --- --- ---
		p2 waterSpeedLocal = -shipSpeedLocal;

		float waterSpeed = magnitude2(waterSpeedLocal);
		p2 waterDir = normalize2(waterSpeedLocal);
		p2 waterDirPerpendicular = { waterDir.y, -waterDir.x };

		float keelAngleOfAttack = -driftAngle;


		// --- --- ---
		// Rudder Angle of Attack
		// --- --- ---
		// DriftAngle is local, so the AoA can be calculated without transforming rudder into global
		float rudderAngleOfAttack = std::remainder(-(rudderAngleLocal + driftAngle), 360.0f);
	}


	float sailIncrease = 0.4; //how much it changes by pressing the keyboard
	float rudderIncrease = 0.2; //how much it changes by pressing the keyboard

	float battery = 1; //Percentage of battery


	p2 position = { 2.128842,41.248926 }; //in LonLats
	p2 finishPoint = { 1.25,39.05 };

	float greatCircleDistance; //spherical distance in meters to finish line
	float totalDistance; //Distance following the nodes


	//Visual heave effect
	p3 shipHeave = { 0,-0.056,0 };
	float shipHeaveIncrease = 0.0002;


	//Ship's parameters, to encapsulate somewhere else
	p3 sailPositionVisual = { 0.602, 0.017, 0 }; //Coordinates where the sail model should be (otherwise is centered on 0)





	Telemetry()
	{
		update();
	}

	void update()
	{
		headingAngle += 0.01;
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


