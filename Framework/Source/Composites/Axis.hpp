#pragma once
#include "Graphics.hpp"

struct Axis 
{
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
	}
	//for centered camera axis
	void drawAux(Shader& shader3D, p3& center)
	{
		opaque();
		shader3D.bind();

		matrix4x4 centerModelMatrix = create3DModelMatrix(0.00001, 0, p3{ 0,0,0 }, center);
		shader3D.setUniform("u_Model", centerModelMatrix);

		shader3D.setUniform("u_fragmentMode", 1);

		glLineWidth(0.2);
		shader3D.setUniform("u_Color", 1.0, 1.0, 1.0, 1.0);
		xLine.draw();
		yLine.draw();
		zLine.draw();

		glLineWidth(1);


	}
};

//centerCross is here I don't know why. Too many files already, and it seems to go hand in hand with axis
struct CenterCross
{
	Lines2D centerCross;

	CenterCross()
	{
		centerCross.addSet({
				{ centerWindow.x - 20, centerWindow.y},{ centerWindow.x + 20, centerWindow.y},
				{ centerWindow.x, centerWindow.y - 20},{ centerWindow.x, centerWindow.y + 20} });
		centerCross.indices = { 0,1,2,3 };
	}

	void draw(Shader& shader2D)
	{
		transparent();
		shader2D.bind();
		shader2D.setUniform("u_Model", identityMatrix);
		shader2D.setUniform("u_Color", 1, 1, 1, 0.5);
		glLineWidth(1);
		centerCross.draw();
		glLineWidth(1);
		opaque();
	}
};