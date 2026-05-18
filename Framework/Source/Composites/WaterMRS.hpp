#pragma once
#include "Graphics.hpp"

struct WaterMRS
{
	//THIS SHOULD BE A POLYGON3D
	Polygons2D circle; 
	Polyhedra water;

	float waterLength = 10;

	WaterMRS()
	{
		circle.createCircle(10, { 0,0 });
		vector<p3> positions = p2ToP3Y(circle.positions);
		vector<p3> normals;

		for (size_t i = 0; i < positions.size(); i++)
		{
			normals.push_back({ 0,1,0 });
		}

		water.addPolyhedra(positions, normals, circle.indices);
	}

	void draw()
	{
		water.draw();
	}
};