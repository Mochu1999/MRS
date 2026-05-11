#pragma once

//Logic should be equal to that of Lines2D but with p3

struct Lines3D
{
	vector<p3> positions;
	vector <unsigned int> indices;

	unsigned int vertexBuffer;
	unsigned int vertexArray;
	unsigned int indexBuffer;

	//if there was only one batch, positions.size() = 3*indices.size()
	size_t positionsDataSize = 0;
	size_t indicesDataSize = 0;
	size_t bufferSize = 0;

	bool isBufferUpdated = false;
	GLenum usageHint = GL_DYNAMIC_DRAW;

	unsigned int indexOffset = 0;

	Lines3D()
	{
		genBuffers();
	}

	~Lines3D();

	void genBuffers();

	void addSet(const vector<p3>& items);

	void createIndices(const vector<p3>& items);

	void draw();

	void clear();
};


