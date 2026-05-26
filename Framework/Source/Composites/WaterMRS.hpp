#pragma once
#include "Graphics.hpp"

struct WaterMRS
{
	//THIS SHOULD BE A POLYGON3D
	Polygons2D circle; 
	Polyhedra water;

	float waterRadius = 7;

	WaterMRS()
	{
		circle.createCircle(waterRadius, { 0,0 });
		vector<p3> positions = p2ToP3Y(circle.positions);
		vector<p3> normals;

		for (size_t i = 0; i < positions.size(); i++)
		{
			normals.push_back({ 0,1,0 });
		}

		water.addPolyhedra(positions, normals, circle.indices);
	}

	void draw(Shader& shader3D)
	{
		transparent();
		shader3D.setUniform("u_Model", identityMatrix);
		shader3D.setUniform("u_fragmentMode", shadeColor);
		shader3D.setUniform("u_Color", 40.0f / 255.0f, 189.9f / 255.0f, 255.0f / 255.0f, 0.6);
		water.draw();
		opaque();
	}
};