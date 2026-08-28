#pragma once


//Same logic as in Lines2D, but input are closed 2D polygons
// Last vertex must be equal to the first one
//Current options are:
//		To pass indices with positions
//		Convex polygons
//		Squares
//		Rounded squares
//TO INTEGRATE COMPLEX TESELATION
struct Polygons2D 
{
	vector<p2> positions;
	vector <unsigned int> indices;

	unsigned int vertexBuffer;
	unsigned int vertexArray;
	unsigned int indexBuffer;


	size_t positionsDataSize = 0;
	size_t indicesDataSize = 0;
	size_t bufferSize = 0;

	bool isBufferUpdated = false;
	GLenum usageHint = GL_DYNAMIC_DRAW;

	unsigned int indexOffset = 0;

	Polygons2D() 
	{
		genBuffers();
	}

	~Polygons2D();

	void genBuffers();

	void addSet(const vector<p2>& positions_);

	//first vertex triangulates with 2 consecutives vertices
	void createConvexIndices(const vector<p2>& positions_);

	void addSet(const vector<p2>& positions_, const vector<unsigned int>& indices_);

	//Specific for squares
	//Heights must be different
	void addRectangle(const p2 c1, const p2 c2);

	void addCircle(const int r, const p2 center, int segments = 0);

	//POLYGONS INSTANCING
	void addRectangle(vector<std::pair<p2, p2>> rectangles);
	void addCircle(const int r, const vector<p2> centers, int segments = 0);

	void draw();

	void clear();
};

/* usage example
Polygons2D polygon;
//convex
polygon.addSet({ {1000,500},{1100,500},{1100,600},{1050,700}, { 1000,600 },{1000,500} });

//indices
polygon.addSet({ {1000,500},{1100,500},{1100,600},{1050,700}, { 1000,600 },{1000,500} }, { 0, 1, 2, 0, 2, 3, 0, 3, 4 });

//rectangles
polygon.addRectangle({ 1000,500 }, { 1100,600 });
polygon.addRectangle({ 1300,500 }, { 1500,600 });

shader2D.bind();
shader2D.setUniform("u_Color", 1, 1, 1, 1);
polygon.draw();
*/