#pragma once

/*usage example

Polyhedra sphere;
	sphere.addSphere(1, { 0,0,0 },1000);

	vector<p2>points = createRandomPoints(150, { 100,100 }, { 600,600 });
	std::vector<unsigned int> indices = delaunayTriangulation(points);

	//debug points
	Polygons2D circles;
	circles.createCircle(7, points);

	//debug triangles
	Lines2D debugDelaunay;
	debugDelaunay.addDelaunaySet(points, indices);

	//draw
	transparent();
	shader3D.bind();
	shader3D.setUniform("u_Color", 1, 0, 0, 1);
	shader3D.setUniform("u_fragmentMode", 1);
	sphere.draw();

	shader2D.bind();
	shader2D.setUniform("u_Color", 1, 0, 0, 1);
	glLineWidth(2);
	circles.draw();
	debugDelaunay.draw();

*/




//Takes a 2D of points mesh and triangulates the mesh into triangle in a way that no
// point falls inside the circumcircle (the circle surrounding a triangle) of any other triangle

//To test the Delaunay algorithm
std::vector<p2> createRandomPoints(int nPoints, p2 minCorner, p2 maxCorner);



struct Triangle 
{
	p2 a, b, c;
	p2 circumcenter;
	float rSquared;

	Triangle(const p2& a, const p2& b, const p2& c) 
		: a(a), b(b), c(c) 
	{
		calculateCircumcenter();
		calculateRSquared();
	}

	void calculateCircumcenter();

	void calculateRSquared();

	//checks if the triangle has a point on its circumcircle
	bool isPointInsideCircumcircle(const p2& point) const;

	//is any point in other triangle equal to a point in this triangle
	bool operator == (Triangle& other);
};

//triangle that guarantees enclosing the full mesh
Triangle createSuperTriangle(const std::vector<p2>& points);






// We have a vector<triangle> with the points triangulated. We could take those with indices 0,1,2-3,4,5-... and call it a day
// But instead of redefing points to do this, we are maintaining them and looking for what's the vector position of each point
// To do that, we store the indices of each point in an unordered map, so we can then itirate through triangles and extract these indices
// simpleIndices would for sure be a faster option
std::vector<unsigned int> extractDelaunayIndices(std::vector<p2>& points, std::vector<Triangle>& triangles);

//very similar to extractDelaunayIndices, but here a vertex in triangles can not be inside point
// and we prevent to add the same index multiple times into indices
std::vector<unsigned int> extractLidIndices(std::vector<p2>& points, std::vector<Triangle>& triangles);


//Extract the triangulated indices of a mesh of 2d points with the Bowyer-Watson algorithm
//Starts with a massive triangle, and progressively breaks it if a point is inside the circumcircle of a triangle (badTriangle)
//When this happens, the outer edges are kept, but the inner ones (that will be repeated) are deleted and the outer ones joint to this new point
//This logic guarantees that no point falls in the circumcircle of any triangle it doesn't share an edge with

//This is the logic of the algorithm, but it is separated in case I want the "lid" points extracted or not
vector<Triangle> internalDelaunayTriangulationAlgorithm(std::vector<p2>& points, Triangle& superTriangle);

//case where I don't want to extract the lid
std::vector<unsigned int> delaunayTriangulation(std::vector<p2>& points);

std::vector<unsigned int> delaunayTriangulationWithLid(std::vector<p2>& points, vector<p3> positions);
