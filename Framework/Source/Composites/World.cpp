#include "World.hpp"

void World::processBinaryMap(const std::string& localPath)
{
	std::string path = "Resources/Map/" + localPath;

	std::ifstream inFile(path, std::ios::binary);
	if (!inFile) {
		std::cerr << "Error opening file for reading: " << path << "\n";
	}

	//iterates through each line and converts it into an inner vector of allLonLats
	while (true)
	{
		size_t size;
		inFile.read(reinterpret_cast<char*>(&size), sizeof(size)); //single polygon size
		if (!inFile) break;  // EOF reached or error

		std::vector<p2> interm(size);
		inFile.read(reinterpret_cast<char*>(interm.data()), size * sizeof(p2));
		if (!inFile) break;

		allLonLats.push_back(std::move(interm));
	}


	//Applies a mercator transformation to allLonLats coordinates
	//Starts in [1] because [0] is the frame
	frame.addSet(lonLatToMercator(allLonLats[0]));
	for (size_t i = 1; i < allLonLats.size(); i++)
	{
		mercator.addSet(lonLatToMercator(allLonLats[i]));
	}

	background.addSet(frame.positions);

}


void World::createSafeZone(float distance)
{
	int count = 0;
	for (auto& polygon : allLonLats)
	{
		int size = polygon.size() - 1;

		vector<p2> interm;

		//Creates a point each iteration of the loop
		for (int i = 0; i < size; i++)
		{
			int prevIndex, nextIndex;

			//method that wraps around first and last point
			prevIndex = (i - 1 + size) % size;
			nextIndex = (i + 1) % size;

			//These 3 points are needed to create each point
			p2 current = polygon[i];
			p2 previous = polygon[prevIndex];
			p2 next = polygon[nextIndex];

			// vectors Previous-Current and Current-Next respectively
			p2 PC = normalize2(previous - current);
			p2 CN = normalize2(next - current);

			p2 bisector = normalize2(PC + CN);

			//bisector will be facing inside if that cross product is smaller than 0
			if (cross2(PC, CN) < 0.0f) bisector = -bisector;

			interm.push_back(current + bisector * distance);
			removeInnerLoops(interm);
		}
		interm.push_back(interm[0]);//first point repeated
		//print(count);
		//print(interm);

		safeZones.push_back(interm);

		//if (count == 10) break;
		count++;
	}

	//Starting with the first point after the frame
	//Pero safeZones no evita ya frame???
	for (size_t i = 1; i < safeZones.size(); i++)
	{
		vector<p2>interm = lonLatToMercator(safeZones[i]);
		safeZonesMercatos.addSet(interm);
	}
}

void World::removeInnerLoops(vector<p2>& polygon)
{
	p2 point; //not needed here

	if (polygon.size() < 2)return;
	size_t j = 0;
	if (polygon.size() > 20) j = polygon.size() - 20;

	for (; j < polygon.size() - 2; j++)
	{
		if (calculateIntersectionPoints(polygon[polygon.size() - 2], polygon[polygon.size() - 1], polygon[j], polygon[j + 1], point))
		{

			polygon.erase(polygon.begin() + j, polygon.begin() + polygon.size() - 1);

			return;
		}
	}
}