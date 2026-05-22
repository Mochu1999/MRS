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

	void draw(Shader& shader2D)
	{
		shader2D.bind();
		transparent();
		shader2D.setUniform("u_Model", identityMatrix);
		shader2D.setUniform("u_Color", 0.035f, 0.065f, 0.085f, 1.0f);
		background.draw();

		shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
		glLineWidth(5);
		shader2D.setUniform("u_Model", identityMatrix);
		backgroundLines.draw();
		glLineWidth(1);
	}
	
};