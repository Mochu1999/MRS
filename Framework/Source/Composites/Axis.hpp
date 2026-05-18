#pragma once
#include "Graphics.hpp"

struct Axis {


	Lines3D xLine, yLine, zLine, xLine2, zLine2;

	Axis()
	{
		xLine.addSet({ {-10000,0,0},{10000,0,0} });
		yLine.addSet({ {0,-10000,0},{0,10000,0} });
		zLine.addSet({ {0,0,-10000},{0,0,10000} });
		xLine2.addSet({ {0,0,0},{5,0,0} });
		zLine2.addSet({ {0,0,0},{0,0,5} });
	}
};

