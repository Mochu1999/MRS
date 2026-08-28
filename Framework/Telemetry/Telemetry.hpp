#pragma once
#include "Time.hpp"
#include "AuxLonLats.hpp"


//SEPARAR TELEMETRÍA CON PARÁMETROS INHERENTES AL BARCO
struct Telemetry
{
	TimeStruct tm;

	p2 position = { 2.128842,41.248926 }; //in LonLats
	p2 finishPoint = { 1.25,39.05 };

	float greatCircleDistance; //spherical distance in meters to finish line
	float totalDistance; //Distance following the nodes

	float headingAngle = 0; //Where the bow is pointing //In dregrees, where north 0º, east 90º...
	p2 shipSpeed; //{surge,sway} with respect of the headingAngle //m/s
	p2 shipSpeedLocal; //surge and sway

	float trueWindSpeed;
	float trueWindAngle;

	float windsSpeed = 0; //m/s
	float windAngle = 0; //º around north

	float sailAngle = 0; //global coordinates
	float sailAngleLocal = 0; //local coordinates
	float sailIncrease = 0.4; //how much it changes by pressing the keyboard

	float rudderAngle = 0;
	float rudderAngleLocal;
	float rudderIncrease = 0.2; //how much it changes by pressing the keyboard

	float battery = 1; //Percentage of battery





	//Visual heave effect
	p3 shipHeave = { 0,-0.056,0 };
	float shipHeaveIncrease = 0.0002;


	//Ship's parameters, to encapsulate somewhere else
	p3 sailPositionVisual = { 0.602, 0.017, 0 }; //Coordinates where the sail model should be (otherwise is centered on 0)

	//VALORES JORGE 3M PARA VALIDACIÓN CON FORCE MODEL DE REZOLA
	/*
	B_one_hull=0.3
	L=3

	# Definición de coordenadas de los apéndices
	sail_pos_m = np.array([1.5, 0, +1.5])
	keel_pos_m = np.array([1.5, 0, -0.3])
	rudder_pos_m = np.array([0.1, 0, -0.15])
	GC_pos_m = np.array([1.3, 0, +0.5])
	*/
	float L = 3;
	float B = 0.3; //one hull
	p3 sailPos = { 1.5,1.5,0 };
	p3 keelPos = { 1.5,-0.3,0 };
	p3 rudderPos = { 0.1,-0.15,0 };
	p3 CG = { 1.3,0.5,0 };


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

	void updateSituation()
	{
		// --- --- ---
	// True wind to global axes
	// --- --- ---
	//We turn trueWindSpeed and trueWindAngle into a single p2 with +x being wind pointing north and +y wind pointing west
	// First we make it negative because it says from where it's coming. if the angle is 0º it's going to -x (south), not to the +x (north)
	//We make a generic vector and then we rotate it
		p2 trueWind = { -trueWindSpeed,0 };
		//Our angle convention goes the other way, 90º cw (east), not ccw (would have been west)
		rotateP2(trueWind, -trueWindAngle);


		// --- --- ---
		// Ship's speed to global axes
		// --- --- ---
		//we maintain speedLocal surge and sway, and we also calculate a global one
		p2 shipSpeed = shipSpeedLocal;
		rotateP2(shipSpeed, -headingAngle);

		float driftAngle = degrees(atan2(shipSpeedLocal.y, shipSpeedLocal.x));//degrees


		// --- --- ---
		// Apparent wind 
		// --- --- ---
		p2 appWind = trueWind - shipSpeed;

		float appWindAngle = degrees(atan2(appWind.y, appWind.x));
		float appWindSpeed = magnitude2(appWind);

		p2 appWindDir = normalize2(appWind);
		p2 appWindDirPerpendicular = { appWindDir.y,-appWindDir.x };


		// --- --- ---
		// Sail Angle of Attack
		// --- --- ---
		// Putting the angle of the sail that we had on global coordinates
		// The negative to follow the angles of the rose, the 180 to flip it so the vector points backwards along the sail (Rezola's AoA sign convention)
		float  sailAngle = 180.0f - (headingAngle + sailAngleLocal);

		// Difference between sailAngle and appWindAngle
		// remainder takes any angle and rewrites it as the equivalent signed angle closest to zero, so range is -180,180 (the closest)
		float sailAngleOfAttack = std::remainder(sailAngle - appWindAngle, 360.0f);


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
};


