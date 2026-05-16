#pragma once






//Siguen habiendo bugs en las tapas


//Creates the positions of a sphere of unit radius centered on O
std::vector<p3> createFibSpherePositions(int nPoints);

//Sets a 2D projection for tessellation
//Input must come have a centered radius (like createFibSpherePositions' output)
std::vector<p2> stereographicProjection(const std::vector<p3>& positions);



void createSphere(float r, p3 center, unsigned int nPositions
	, vector<p3>& positions, vector<p3>& normals, vector<unsigned int>& indices);

