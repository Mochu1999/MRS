#pragma once


//The output we want is: Fx, Fy, Mz
//Everything in SI except the heading and trueWind and sail angles that are on degrees
//All angles are in global coordinates unless it says they are local

bool calculateLiftAndDrag(float& lift, float& drag, float aoa, float speed);

void forceModel(Parameters& p)
{
	p.calculateVariables();


	//auto& [sailAngle, rudderAngle, windSpeed] = t;

	//N
	float sailLift = 0, sailDrag = 0;
	calculateLiftAndDrag(sailLift, sailDrag, p.sailAoA, p.appWindSpeedMag);
	float keelLift = 0, keelDrag = 0;
	calculateLiftAndDrag(keelLift, keelDrag, p.keelAoA, p.waterSpeedMag);
	float rudderLift = 0, rudderDrag = 0;
	calculateLiftAndDrag(rudderLift, rudderDrag, p.rudderAoA, p.waterSpeedMag);


	//--- --- ---
	// Forces
	//--- --- ---
	p2 sailForce = p.appWindDir * sailDrag + p.appWindDirPerpendicular * sailLift;
	p2 sailForceLocal = sailForce;
	//Getting a global from a local: positive; A local from a global: negative
	// Because if a global speed is {0,1} with 90º heading, locally it's {1,0} (full forward, as it was pointing north). That's a CW (negative) -90º rotation
	rotateP2(sailForceLocal, -p.headingAngle);

	p2 keelForce = p.waterSpeedDir * keelDrag + p.waterSpeedDirPerpendicular * keelLift;
	p2 keelForceLocal = keelForce;
	rotateP2(keelForceLocal, -p.headingAngle);

	p2 rudderForce = p.waterSpeedDir * rudderDrag + p.waterSpeedDirPerpendicular * rudderLift;
	p2 rudderForceLocal = rudderForce;
	rotateP2(rudderForceLocal, -p.headingAngle);

	//--- --- ---
	// Moment
	//--- --- ---
	// Mz = rx * Fy + ry * (-Fx)
	// Or in this framework axes: My = rx * Fz - rz * Fx
	//But as our appendages doesn't have lateral coordinates Myaw = rx * Fz
	p3 rudderDistance = p.rudderPos - p.CG;
	p3 keelDistance = p.keelPos - p.CG;
	p3 sailDistance = p.sailPos - p.CG;

	//It isn't on z but whatever
	float MzRudder = rudderForceLocal.y * rudderDistance.x;
	float MzKeel = keelForceLocal.y * keelDistance.x;
	float MzSail = sailForceLocal.y * sailDistance.x;

	float Myaw = MzRudder + MzKeel + MzSail;









	/*float Xvv_p = -0.1775;
	float Yv_p = -0.5833;
	float Yvvv_p = -8.1093;
	float Nv_p = -0.2647;
	float Nvvv_p = -2.8373;
	
	float Xvv = Xvv_p * 0.5 * rhoWater * p.L * p.BreadthSingleHull;
	float Yv = Yv_p * 0.5 * rhoWater * p.L * p.BreadthSingleHull * Vboat_norm;
	float Yvvv = Yvvv_p * 0.5 * rhoWater * p.L * p.BreadthSingleHull / Vboat_norm;
	float Nv = Nv_p * 0.5 * rhoWater * p.L * p.L * p.BreadthSingleHull * Vboat_norm;
	float Nvvv = Nvvv_p * 0.5 * rhoWater * p.L * p.L * p.BreadthSingleHull / Vboat_norm;*/
}


//NLLT (Nonlinear Lifting Line Theory) solver from Alberto 
//True if the solution converges
bool calculateLiftAndDrag(float& lift, float& drag, float aoa, float speed)
{

	return true;
}