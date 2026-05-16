#pragma once

#include "KeyMouseImputs.hpp"

#include "Time.hpp"

struct Telemetry
{
	
	TimeStruct tm;
	

	
	Telemetry()
		
	{

		
	}

	void update()
	{
		tm.update();


		
	}
};