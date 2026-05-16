#include "Graphics.hpp"

std::vector<p2> createRandomPoints(int nPoints, p2 minCorner, p2 maxCorner)
{
	// Seed for random number generation
	std::random_device rd;
	std::mt19937 gen(rd());

	// Distributions for x and y coordinates separetely
	std::uniform_real_distribution<float> disX(minCorner.x, maxCorner.x);
	std::uniform_real_distribution<float> disY(minCorner.y, maxCorner.y);

	std::vector<p2> points;
	points.reserve(nPoints);

	for (int i = 0; i < nPoints; ++i)
	{
		points.push_back(p2{ disX(gen), disY(gen) });
	}

	return points;
}



void Triangle::calculateCircumcenter()
{
	//we are calculating it with respect to a
	// so ab is the local coordinate of b, ac of c
	p2 ab = b - a;
	p2 ac = c - a;

	//The final formula is derived from the equations
	// dot(c,ab) = |ab|^2/2
	// dot(c,ac) = |ac|^2/2
	//that says that the circumcenter lies on the perpendicular bisector of each edge
	// | c - ab | ^ 2 = (c - ab)·(c - ab) = c·c - 2c·ab + ab·ab

	float abSquared = dot2(ab, ab);
	float acSquared = dot2(ac, ac);

	//cross2 is the area of the parallelogram ab-ac
	float denominator = 2.0f * cross2(ab, ac);

	p2 localCircumcenter =
	{ (ac.y * abSquared - ab.y * acSquared) / denominator,
	(ab.x * acSquared - ac.x * abSquared) / denominator };

	circumcenter = a + localCircumcenter;
}

void Triangle::calculateRSquared()
{
	p2 distance = circumcenter - a;
	rSquared = dot2(distance, distance); //|distance|^2 as vectors have the same direction
}

//checks if the triangle has a point on its circumcircle
bool Triangle::isPointInsideCircumcircle(const p2& point) const
{
	p2 distance = point - circumcenter;
	return dot2(distance, distance) <= rSquared;
}

//is any point in other triangle equal to a point in this triangle
bool Triangle::operator == (Triangle& other)
{
	return
		(a == other.a || a == other.b || a == other.c ||
			b == other.a || b == other.b || b == other.c ||
			c == other.a || c == other.b || c == other.c);
}


Triangle createSuperTriangle(const std::vector<p2>& points)
{
	//bounding box limits
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float maxY = std::numeric_limits<float>::lowest();
	for (const p2& point : points)
	{
		minX = std::min(minX, point.x);
		minY = std::min(minY, point.y);
		maxX = std::max(maxX, point.x);
		maxY = std::max(maxY, point.y);
	}

	float width = maxX - minX;
	float height = maxY - minY;
	float centerX = (minX + maxX) * 0.5f;

	float size = std::max(width, height);

	//This geometric rule works for all bounding boxes whatever their ratio
	p2 a{ centerX - 2 * size, minY - size };
	p2 b{ centerX + 2 * size, minY - size };
	p2 c{ centerX, maxY + 2 * size };

	return Triangle(a, b, c);
}


std::vector<unsigned int> extractDelaunayIndices(std::vector<p2>& points, std::vector<Triangle>& triangles)
{
	std::vector<unsigned int> indices;
	std::unordered_map<p2, unsigned int, p_HashMultiplicative> uMapPositions;

	uMapPositions.reserve(points.size());
	indices.reserve(triangles.size() * 3);

	//key is the position (points[i]), the stored value i
	for (unsigned int i = 0; i < points.size(); ++i)
	{
		uMapPositions[points[i]] = i;
	}

	//inserting the stored value of the map of each point, that's their indices
	for (const auto& triangle : triangles)
	{
		indices.insert(indices.end(), { uMapPositions[triangle.a],uMapPositions[triangle.b],uMapPositions[triangle.c] });
	}

	return indices;
}

vector<Triangle> internalDelaunayTriangulationAlgorithm(std::vector<p2>& points, Triangle& superTriangle)
{
	std::vector<Triangle> triangles;
	triangles.reserve(2 * points.size()); //there's an Euler formula that says that it will be close but less than 2
	triangles.push_back(superTriangle);

	//We only loop once through points, wherever the point is, the triangles that contain it will be broken down
	for (const p2& point : points)
	{
		//triangles that contain a point inside its circumcircle
		std::vector<Triangle> badTriangles;
		badTriangles.reserve(triangles.size());

		//triangles that do not contain any point
		//we could just use the variables triangles and badTriangles, but we keep this to avoid erasing mid vector
		std::vector<Triangle> goodTriangles;
		goodTriangles.reserve(triangles.size());

		//Now we see if our point is inside any triangle
		for (const Triangle& triangle : triangles)
		{
			if (triangle.isPointInsideCircumcircle(point))
				badTriangles.push_back(triangle);
			else
				goodTriangles.push_back(triangle);
		}
		//we end with a triangles with removed badTriangles
		triangles = std::move(goodTriangles);


		//From this bad triangles, we do substanct its edges
		//This edges can only appear one or twice
		//If they appear once, it is because they are a boundary and we want to keep these edges
		//If they appear twice, then we remove them
		std::unordered_set<std::pair<p2, p2>, pair_hash_multiplicative> edgeContainer;

		for (const Triangle& tr : badTriangles)
		{
			std::pair<p2, p2> edges[3] = { {tr.a, tr.b}, {tr.b, tr.c}, {tr.c, tr.a} };

			for (const auto& edge : edges)
			{
				// We need to guarantee that edges are ordered the same way so we can find coincidences
				// Making smaller edge.first going first
				std::pair<p2, p2> normalizedEdge = edge.first < edge.second ? edge : std::pair<p2, p2>(edge.second, edge.first);

				//this is a structured binding, after inserting we get an iterator of the inserted element and if it succeeded inserting the edge
				auto [it, succeedInserting] = edgeContainer.insert(normalizedEdge);
				if (!succeedInserting)
				{
					edgeContainer.erase(it);
				}
			}
		}

		//Now we add into triangles the bad triangles refactored
		triangles.reserve(triangles.size() + edgeContainer.size());

		for (const auto& edge : edgeContainer)
		{
			triangles.emplace_back(point, edge.first, edge.second);
		}
	}

	return triangles;
}

std::vector<unsigned int> extractLidIndices(std::vector<p2>& points, std::vector<Triangle>& triangles)
{
	std::vector<unsigned int> indices;
	std::unordered_map<p2, unsigned int, p_HashMultiplicative> uMapPositions;
	std::unordered_set<unsigned int> addedIndices; //to track if the index has been already added

	uMapPositions.reserve(points.size());
	addedIndices.reserve(points.size());
	indices.reserve(triangles.size() * 3);

	for (unsigned int i = 0; i < points.size(); ++i)
	{
		uMapPositions[points[i]] = i;
	}

	//adding a index only if it is indeed part of points (not from superTriangle) and is not already in indices
	for (const auto& triangle : triangles)
	{
		auto itA = uMapPositions.find(triangle.a);
		if (itA != uMapPositions.end() &&
			addedIndices.find(itA->second) == addedIndices.end())
		{
			indices.push_back(itA->second);
			addedIndices.insert(itA->second);
		}

		auto itB = uMapPositions.find(triangle.b);
		if (itB != uMapPositions.end() &&
			addedIndices.find(itB->second) == addedIndices.end())
		{
			indices.push_back(itB->second);
			addedIndices.insert(itB->second);
		}

		auto itC = uMapPositions.find(triangle.c);
		if (itC != uMapPositions.end() &&
			addedIndices.find(itC->second) == addedIndices.end())
		{
			indices.push_back(itC->second);
			addedIndices.insert(itC->second);
		}
	}

	return indices;
}