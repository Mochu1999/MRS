#pragma once
#include "Graphics.hpp"


struct SunMRS
{
	Polyhedra sun;

	p3 sunPos = { 20,15,25 };

	SunMRS()
	{
		sun.addSphere(2, sunPos);
	}

	void draw()
	{
		sun.draw();
	}
};