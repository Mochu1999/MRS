#pragma once
#include "Time.hpp"
#include "AuxLonLats.hpp"


struct Parameters
{
	TimeStruct tm;

	//all angles in degrees, the rest in IS
	
	//internally north 0º and CCW from there. West 90º, South 180º, East = 270º
	float headingAngle = 0; //Where the bow is pointing

	p2 shipSpeedLocal; //{surge,sway} with respect of the headingAngle //m/s
	p2 shipSpeed; // speed in global axis

	float waterSpeedMag = 0; //-shipSpeed, for calculateLiftAndDrag
	p2 waterSpeedDir;
	p2 waterSpeedDirPerpendicular; //for forces

	float driftAngle = 0; //0º is there's no sway


	p2 trueWind; //global axis
	float trueWindSpeedMag = 0; //m/s
	float trueWindAngle = 0; //º around north

	p2 appWind; //apparent wind
	float appWindSpeedMag = 0;
	p2 appWindDir; //unitary value of appWind
	p2 appWindDirPerpendicular;

	float sailAngle = 0; //global axis //90º means that the leading edge looks west, and the trailing edge looks east
	float sailAngleLocal = 0; 

	float rudderAngleLocal = 0; 
	float rudderAngle = 0;


	//Angles from flow direction to foil direction
	float sailAoA = 0; // Difference between trailing edge of sailAngle and appWindAngle
	float keelAoA =0;
	float rudderAoA = 0;

	//Valores barco 3 metros rezola
	float L = 3;
	float BreadthSingleHull = 0.3; //each hull
	//float B = 0.3; //one hull
	p3 sailPos = { 1.5,1.5,0 };
	p3 keelPos = { 1.5,-0.3,0 };
	p3 rudderPos = { 0.1,-0.15,0 };
	p3 CG = { 1.3,0.5,0 };


	//ESTO ES CON LAS HIPOTESIS QUE HIZO REZOLA
	//Las variables que conoceremos por sensor serán:
	// --- --- ---
	// appWindSpeedMag, appWindAngle
	// shipSpeed, headingAngle
	// sailAngleLocal, rudderAngleLocal
	// --- --- ---
	// CalculateVariables debería de sacar todas las demás a partir de esas
	 void calculateVariables();


	

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





	Parameters()
	{
		vector<pair<float, float>> hullResistances;
		readResistancesText("Resources/ForceModel/resistencia_jorge_ms.txt", hullResistances);
		print(hullResistances.size());
		print(hullResistances);


		update();
	}

	void update()
	{
		//headingAngle += 0.01;
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


