#include "Graphics.hpp"

std::vector<p3> createFibSpherePositions(int nPoints)
{
	std::vector<p3> positions;
	positions.reserve(nPoints); //number of points

	//points are separated horizontally at an angle (the golden angle) from the previous point. 
	const float goldenAngle = PI * (3.0 - std::sqrt(5.0));
	float nInv = 1.0f / nPoints;

	for (int i = 0; i < nPoints; ++i)
	{
		float y = -1.0 + (2.0 * i + 1.0) * nInv; //Points are distributed vertically from -1 to 1, evenly spaced

		// Calculate radius at this height to the vertical axis to ensure points lie on the sphere surface
		float r = std::sqrt(1.0 - y * y);

		float theta = goldenAngle * i;

		float x = r * std::cos(theta);
		float z = r * std::sin(theta);

		//scaled them to their real radius
		positions.emplace_back(p3{ x, y , z });
	}

	return positions;
}

std::vector<p2> stereographicProjection(const std::vector<p3>& positions)
{
	std::vector<p2> projectedPositions;
	projectedPositions.reserve(positions.size());

	for (const p3& point : positions)
	{
		p3 normalizedPosition = normalize3(point);

		float projectionScale = 2.0 / (1.0 + normalizedPosition.y);  // Standard stereographic projection formula

		projectedPositions.emplace_back(p2{ projectionScale * normalizedPosition.x,projectionScale * normalizedPosition.z });
	}

	return projectedPositions;
}

void createSphere(float r, p3 center, unsigned int nPositions
	, vector<p3>& positions, vector<p3>& normals, vector<unsigned int>& indices)
{
	positions.clear();
	normals.clear();
	indices.clear();

	if (nPositions == std::numeric_limits<unsigned int>::max())
	{
		//nPositions = 1024 * 1024 / 3 / sizeof(p3); //I want positions.size to be 1/3 of a binary MB
		nPositions = 10000 / 3 / sizeof(p3);
	}


	////Creating the positions of a unit sphere
	positions = createFibSpherePositions(nPositions);


	////Creating indices
	//tessellating the 2D sphere
	std::vector<p2> projectedPoints = stereographicProjection(positions);

	vector<unsigned int> debugIndices;
	indices = delaunayTriangulationWithLid(projectedPoints, positions);

	//MOVING POSITIONS WITH THE CPU
	for (int i = 0; i < positions.size(); i++)
	{
		positions[i] *= r;
		positions[i] += center;
	}


	//Creating normals
	for (auto& pos : positions)
	{
		normals.push_back(normalize3(pos - center));
	}
}