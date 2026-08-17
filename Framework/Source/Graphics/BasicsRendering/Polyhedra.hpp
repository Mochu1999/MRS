#pragma once

#include "FilesManagement.hpp"

//TO DO: Instancing polyhedra

//To have sharp illumination, you need to repeat positions to have different normals per vertex.
//If you don't repeat positions you can only have one normal per vertex (soft illumination)

//one object per polyhedra, no batches
//Can directly read custom .bin models
struct Polyhedra
{
	vector<p3> positions;
	vector<unsigned int> indices;
	vector<p3> normals;

	unsigned int vertexArray;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
	unsigned int normalsBuffer;


	bool isBufferUpdated = true;
	GLenum usageHint = GL_STATIC_DRAW;

	Polyhedra()
	{
		genBuffers();
	}

	~Polyhedra();

	void genBuffers();

	//Simple Polyhedra format
	void addPolyhedra(const std::string& localPath);

	void addPolyhedra(const vector<p3>& positions_, const vector<p3>& normals_, const vector<unsigned int>& indices_);

	void addSTL(const std::string& localPath)
	{
		clear();

		readSTL(localPath, positions, normals, indices);

		isBufferUpdated = true;
	}

	void addSphere(float r, p3 center, unsigned int nPositions = std::numeric_limits<unsigned int>::max())
	{
		createSphere(r, center, nPositions, positions, normals, indices);

		isBufferUpdated = true;
	}

	void draw();

	void clear();
};

/* usage example

stlToSimplePolyhedra("pendulum1.stl", "pendulum1.bin");
Polyhedra polyhedra;
polyhedra.addPolyhedra("pendulum1.bin");

opaque();
shader3D.bind();
shader3D.setUniform("u_Color", 1, 1, 1, 1);
shader3D.setUniform("u_Model3D", identityMatrix);
shader3D.setUniform("u_fragmentMode", 0);
polyhedra.draw();

*/

//Passes the data from a polyhedra to a polygon by removing the y data from positions and the normals
static void polyhedraTo2D(Polyhedra polyhedra, Polygons2D& polygon)
{
	vector<p2> positions2d;

	for (auto& pos : polyhedra.positions)
	{
		positions2d.push_back({ pos.x,pos.z });
	}
	polygon.addSet(positions2d);

	////In this iteration addSet only works for convex
	polygon.indices = polyhedra.indices;
}