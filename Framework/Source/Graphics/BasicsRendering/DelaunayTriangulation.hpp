#pragma once

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
inline std::vector<unsigned int> delaunayTriangulation(std::vector<p2>& points) 
{
	//Englobes every point
	Triangle superTriangle = createSuperTriangle(points);

	//the logic of the algorithm is inside this function, it returns the points triangulated but with the superTriangle attached
	std::vector<Triangle> triangles = internalDelaunayTriangulationAlgorithm(points, superTriangle);

	//removing triangles that contain a point of superTriangle
	for (auto it = triangles.begin(); it != triangles.end(); )
	{
		if (*it == superTriangle)
			it = triangles.erase(it);
		else
			++it;
	}


	//and we return the indices of these triangles
	return extractDelaunayIndices(points, triangles);
}





inline std::vector<unsigned int> delaunayTriangulationWithLid(std::vector<p2>& points, vector<p3> positions)
{
	vector<unsigned int> indices;

	Triangle superTriangle = createSuperTriangle(points);

	std::vector<Triangle> triangles = internalDelaunayTriangulationAlgorithm(points, superTriangle);

	//we remove triangles that touch superTriangle, but we keep them for the lid
	vector<Triangle> lidTriangles;
	for (auto it = triangles.begin(); it != triangles.end(); )
	{
		if (*it == superTriangle)
		{
			lidTriangles.push_back(*it);
			it = triangles.erase(it);
		}
		else
			++it;
	}
	//normal indices from triangles
	indices = extractDelaunayIndices(points, triangles);


	//returns the indices of the lidTriangles without the superTriangle vertices and no repeated vertex
	vector<unsigned int> lidIndices = extractLidIndices(points, lidTriangles);

	//we create a new vector of points for triangulation
	//WE COULD USE POINTS, BUT DIRECT PROJECTION OF THE 3D POINTS CREATE A LITTLE LESS PROBLEMS THAN STEREOGRAPHIC
	vector<p2> lidPoints;
	for (size_t i = 0; i < lidIndices.size(); i++)
	{
		lidPoints.push_back(p2{ positions[lidIndices[i]].x,positions[lidIndices[i]].z });
	}
	//THE LID SHOULDN'T BE TRIANGULATED WITH DELAUNAY
	vector<unsigned int> newIndices = delaunayTriangulation(lidPoints);

	for (size_t i = 0; i < newIndices.size(); i++)
	{
		newIndices[i] = lidIndices[newIndices[i]];
	}
	//MAKING THEM CHANGE ORIENTATION DOENS'T CHANGE THE COLOR BUG PROBLEM
	/*for (size_t i = 0; i < newIndices.size(); i += 3)
	{
		std::swap(newIndices[i + 1], newIndices[i + 2]);
	}*/


	indices.insert(indices.end(), newIndices.begin(), newIndices.end());

	return indices;
}
