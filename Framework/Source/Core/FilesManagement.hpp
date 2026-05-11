#pragma once

// --- --- ---
// STL - SimplePolyhedra
// --- --- ---

void readSTL(const std::string& localPath
	, std::vector<p3>& positions, std::vector<p3>& normals, std::vector<unsigned int>& indices);

void readSimplePolyhedra(const std::string& localPath
	, std::vector<p3>& positions, std::vector<p3>& normals, std::vector<unsigned int>& indices);

void writeSimplePolyhedra(const std::string& localPath
	, std::vector<p3>& positions, std::vector<p3>& normals, std::vector<unsigned int>& indices);

// ascii stls to custom SimplePolyhedra format
void stlToSimplePolyhedra(const std::string& inputLocalPath, const std::string& outputLocalPath);
// stlToBinary("pendulum1.stl", "pendulum1.bin");





///// Split a string by a delimiter (e.g. ',') into a list of tokens
//inline std::vector<std::string> split(const std::string& str, char delimiter) {
//	std::vector<std::string> tokens;
//	std::stringstream ss(str);
//	std::string item;
//	while (std::getline(ss, item, delimiter)) {
//		tokens.push_back(item);
//	}
//	return tokens;
//}
//
///// Given a LINESTRING-like substring "1.0 2.0, 3.1 4.2, ..."
///// parse out each "lon lat" and convert to p2
//inline std::vector<p2> parseLineStringCoords(const std::string& coordString) {
//	std::vector<p2> points;
//	// split by comma -> ["1.0 2.0", "3.1 4.2", ...]
//	auto commaSeparated = split(coordString, ',');
//	for (auto& cs : commaSeparated) {
//		std::stringstream ss(cs);
//		float lon, lat;
//		ss >> lon >> lat;  // read "1.0 2.0" etc.
//		points.push_back({ lon, lat });
//	}
//	return points;
//}








//I've made as an intermediate step to modify the csv data of the map, maybe delete it in the future?
//It's in a weird format, in my head it makes much more sense to store all positions sizes at the start and then all the positions
//, but I need to modify a lot of data, so here we are
inline void writeVectorOfVectors(const vector<vector<p2>>& model) {
	std::string path = "Resources/Borrar/map.txt";

	std::cout << "Setting model in: " << path << std::endl;

	std::ofstream outFile(path, std::ios::binary);
	if (outFile)
	{
		for (size_t i = 0; i < model.size(); i++)
		{
			size_t size = model[i].size();
			outFile.write(reinterpret_cast<const char*>(&size), sizeof(size_t));
			outFile.write(reinterpret_cast<const char*>(model[i].data()), size * sizeof(p2));
		}
	}
	else
	{
		std::cout << "Error opening file for writing." << std::endl;
	}
	outFile.close();
}

inline void writeVectorOfVectorsAscii(const std::vector<std::vector<p2>>& model)
{
	std::string path = "Resources/Borrar/map.txt";
	std::ofstream outFile(path);
	if (!outFile)
	{
		std::cout << "Error opening file for writing.\n";
		return;
	}


	for (size_t i = 0; i < model.size(); ++i)
	{
		outFile << model[i].size() << " ";

		for (size_t j = 0; j < model[i].size(); ++j)
		{
			outFile << model[i][j].x << " " << model[i][j].y << ", ";
		}

		outFile << "\n";
	}

	outFile.close();
}

