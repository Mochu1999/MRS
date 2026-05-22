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

	void draw(Shader& shader3D)
	{
		opaque();
		shader3D.bind();
		shader3D.setUniform("u_Model", identityMatrix);

		shader3D.setUniform("u_fragmentMode", 1);


		glLineWidth(1);
		shader3D.setUniform("u_Color", 1.0, 0.0, 0.0, 1.0);
		xLine.draw();
		shader3D.setUniform("u_Color", 0.0, 1.0, 0.0, 1.0);
		yLine.draw();
		shader3D.setUniform("u_Color", 0.0, 0.0, 1.0, 1.0);
		zLine.draw();
		shader3D.setUniform("u_Color", 1, 1, 1, 1.0);

		glLineWidth(1);

		shader3D.setUniform("u_Color", 1, 1, 1, 1.0);

	}
};

