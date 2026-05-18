#pragma once

#include "Graphics.hpp"


struct Overlay2D 
{
	Polygons2D background;
	Lines2D backgroundLines;

	Overlay2D()
	{

		p2 corner = { 1300,0 };
		vector<p2> arc = createRoundedSquare({ corner.x,0 }, windowWidth - corner.x, 840 , 25);
		background.addSet(arc);
		backgroundLines.addSet(arc);
	}


	
};