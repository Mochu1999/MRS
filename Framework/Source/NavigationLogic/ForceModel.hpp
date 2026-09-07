#pragma once


//The output we want is: Fx, Fy, Mz
//Everything in SI except the heading and trueWind and sail angles that are on degrees
//All angles are in global coordinates unless it says they are local



void forceModel(Telemetry& t)
{
	t.updateSituation();
	



}