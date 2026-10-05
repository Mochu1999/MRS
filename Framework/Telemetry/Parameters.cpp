#include "Parameters.hpp"

void Parameters::calculateVariables()
{
	// --- --- ---
	// Rudder and sail global angles
	// --- --- ---
	
	sailAngle = headingAngle + sailAngleLocal;
	rudderAngle = headingAngle + rudderAngleLocal;

	// --- --- ---
	// True wind to global axes
	// --- --- ---
	//We turn trueWindSpeedMag and trueWindAngle into a single p2 with +x being wind pointing north and +y wind pointing west
	// First we make it negative because it points towards where it's coming. if the angle is said to be 0º in reality it's going to -x south (180), not to the 0º +x north
	//We make a generic vector and then we rotate it
	trueWind = { -trueWindSpeedMag,0 };
	
	// CCW angles are positive (unlike Rezola's)
	//I am getting a Global speed from a local one. If locally it's {1,0} (no sway) and 90º of angle, globally it's {0,1}, a CCW (positive) 90º rotation
	rotateP2(trueWind, trueWindAngle);


	// --- --- ---
	// Ship's speed to global axes
	// --- --- ---
	//we calculate the global one from the known local one
	shipSpeed = shipSpeedLocal;
	rotateP2(shipSpeed, headingAngle);

	driftAngle = degrees(atan2(shipSpeedLocal.y, shipSpeedLocal.x));


	// --- --- ---
	// Apparent wind 
	// --- --- ---
	appWind = trueWind - shipSpeed;
	appWindSpeedMag = magnitude2(appWind);
	float appWindAngle = degrees(atan2(appWind.y, appWind.x));
	//float appWindSpeedMag = magnitude2(appWind);

	appWindDir = normalize2(appWind);
	appWindDirPerpendicular = { appWindDir.y,-appWindDir.x };


	// --- --- ---
	// Angles of Attack
	// --- --- ---
	//Angles from incoming flow direction to foil direction

	// Difference between sailAngle and appWindAngle
	// Remainder 360 moves the float to a [-180,180]. 170º stays 170, while 190 turns into -170. -330 would be 30º
	// 180 to flip it so the vector points backwards along the sail (AoA sign convention from Rezola uses trailing edge)
	sailAoA = std::remainder(sailAngle +180 - appWindAngle, 360.0f);

	//Drift angle is already the difference between water speed and the keel angle (0 locally, heading globally)
	// So we don't use global axis like for sail, and we don't need to calculate a global water speed and it's angle like Rezola
	//No need to use remainder because atan2 already gave it in that range
	keelAoA = -driftAngle;

	// Like the keel angle of attack we can work in local coordinate
	//Doesn't need a 180 because the water speed is pointing backwards
	rudderAoA = std::remainder( rudderAngleLocal - driftAngle, 360.0f);




	waterSpeedMag = magnitude2(-shipSpeed); //the minus is for clarity, irrelevant in the calculation
	waterSpeedDir = normalize2(-shipSpeed);
	waterSpeedDirPerpendicular = { waterSpeedDir.y, -waterSpeedDir.x };
}

