#pragma once

#include "FilesManagement.hpp"

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

	void addPolyhedra(const std::string& localPath);

	void addPolyhedra(const vector<p3>& positions_, const vector<unsigned int>& indices_, const vector<p3>& normals_);

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