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

	void draw(Shader& shader3D)
	{
		shader3D.bind();
		opaque();
		shader3D.bind();
		shader3D.setUniform("u_Model", identityMatrix);
		shader3D.setUniform("u_fragmentMode", flatColor);
		shader3D.setUniform("u_Color", 1.0, 0.60, 0.05, 1);
		sun.draw();
	}

	void updateLightLocation(Shader& shader3D)
	{
		shader3D.bind();
		shader3D.setUniform("u_lightPos", sunPos);
	}
};