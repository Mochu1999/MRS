#pragma once


//draw lines in positions batches
struct Lines2D
{
	vector<p2> positions;
	vector <unsigned int> indices;


	unsigned int vertexArray;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;


	//drawing buffers sizes (bytes)
	//if there was only one batch, positions.size() = 2*indices.size()
	// This breaks when multiple batches have different sizes
	size_t positionsDataSize = 0;
	size_t indicesDataSize = 0;
	size_t bufferSize = 0;

	bool isBufferUpdated = false;
	GLenum usageHint = GL_DYNAMIC_DRAW;

	//separates batches
	unsigned int indexOffset = 0;


	Lines2D()
	{
		genBuffers();
	}
	~Lines2D();

	void genBuffers();

	//Batches vertices are linked one after another
	void addSet(const vector<p2>& items);

	void createRectangle(const p2 corner1, const p2 corner2);

	void createRectangle(vector<std::pair<p2, p2>> rectangles);

	void createCircle(const int r, const p2 center, int segments = 0);

	void createCircle(const int r, const vector<p2> centers, int segments = 0);

	

	void createIndices(const vector<p2>& items);

	void draw();

	void clear();

	//to debug delaunay meshes. usage example in Delaunay
	void addDelaunaySet(const vector<p2>& positions_, const vector<unsigned int>& indices_)
	{
		clear();
		for (size_t i = 0; i < indices_.size(); i += 3)
		{
			p2 a = positions_[indices_[i]];
			p2 b = positions_[indices_[i + 1]];
			p2 c = positions_[indices_[i + 2]];
			addSet(vector<p2>{a, b, c, a});
		}
	}
};

/*usage example

Lines2D lines;
lines.clear();
lines.addSet({ {0,0},{1000,1000},{1000,0} });
lines.addSet({ {1500,1000},{1500,0} });

shader2D.bind();
shader2D.setUniform("u_Color", 1, 1, 1);
glLineWidth(5);
lines.draw();
glLineWidth(1);
*/



