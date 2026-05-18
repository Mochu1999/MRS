#include "Graphics.hpp"

vector<p2> createRectanglePositions(const p2 corner1, const p2 corner2)
{
	vector<p2> positions;

	float xMin = min(corner1.x, corner2.x);
	float xMax = max(corner1.x, corner2.x);
	float yMin = min(corner1.y, corner2.y);
	float yMax = max(corner1.y, corner2.y);

	vector<p2> positions_ = { p2{ xMin,yMin }, p2{ xMax,yMin }, p2{xMax,yMax },p2{xMin,yMax}, p2{ xMin,yMin } };
	positions.insert(positions.end(), positions_.begin(), positions_.end());

	return positions;
}

vector<p2> createCirclePositions(const int r, const p2 center, int segments)
{
	vector<p2> positions;

	if (segments == 0)
		segments = r * 100;

	//angle between points in radians
	float angleSegment = 2 * PI / segments;
	float theta;


	positions.reserve(segments + 1);

	//starts in 0º,
	for (int i = 0; i < segments + 1; i++)
	{
		theta = (angleSegment * i);
		positions.push_back(p2{ center.x + r * cos(theta), center.y + r * sin(theta) });
	}

	return positions;
}
vector<p2> createArc(p2 center, float r, float angle1, float angle2, int segments = 0)
{

	vector<p2> positions;
	positions.reserve(segments + 1);

	while (angle1 > angle2)
	{
		angle2 += 2 * PI;
	}



	if (segments < 1)
	{
		segments = r * 3 * (angle2 - angle1) / (2 * PI);

		if (segments < 10) //it assumes that if you've put anything less than 10 you know why
			segments = 10;
	}

	float angleSegment = (angle2 - angle1) / segments; //angle bewteern points in radians


	for (int i = 0; i < segments + 1; i++)
	{
		float theta = angle1 + angleSegment * i;
		positions.push_back(p2{ center.x + r * cos(theta), center.y + r * sin(theta) });
	}


	return positions;
}

vector<p2> createRoundedSquare(p2 startingPos, float width, float height, float radius) {
	vector<p2> positions;
	//reserve


	//bottom left
	vector<p2> temp = createArc(startingPos + radius, radius, radians(180), radians(270));
	positions.insert(positions.end(), temp.begin(), temp.end());

	temp = createArc({ startingPos.x + width - radius,startingPos.y + radius }, radius, radians(270), 0);
	positions.insert(positions.end(), temp.begin(), temp.end());

	temp = createArc({ startingPos.x + (width - radius),startingPos.y + (height - radius) }, radius, 0, radians(90));
	positions.insert(positions.end(), temp.begin(), temp.end());

	temp = createArc({ startingPos.x + radius,startingPos.y + height - radius }, radius, radians(90), radians(180));
	positions.insert(positions.end(), temp.begin(), temp.end());

	positions.emplace_back(positions.front());

	return positions;
}

vector<p2> createRoundedSquare(p2 startingPos, p2 endingPos, float radius)
{
	vector<p2> positions;

	float width = endingPos.x - startingPos.x;
	float height = endingPos.y - startingPos.y;

	// bottom left
	vector<p2> temp = createArc({ startingPos.x + radius, startingPos.y + radius }, radius, radians(180), radians(270));
	positions.insert(positions.end(), temp.begin(), temp.end());

	// bottom right
	temp = createArc({ endingPos.x - radius, startingPos.y + radius }, radius, radians(270), 0);
	positions.insert(positions.end(), temp.begin(), temp.end());

	// top right
	temp = createArc({ endingPos.x - radius, endingPos.y - radius }, radius, 0, radians(90));
	positions.insert(positions.end(), temp.begin(), temp.end());

	// top left
	temp = createArc({ startingPos.x + radius, endingPos.y - radius }, radius, radians(90), radians(180));
	positions.insert(positions.end(), temp.begin(), temp.end());

	positions.emplace_back(positions.front());

	return positions;
}









