#pragma once

#include <filesystem>
namespace fs = std::filesystem;
#include "Common.hpp"
#include "Polyhedra.hpp"

//A lo mejor meter esto en binaries manager. En la versión 2d queda más bonito

void writeModel(const std::vector<p3>& model) {
	std::string basePath = "Resources/Plain models/";
	std::string modelPath;

	std::cout << "Current models: " << std::endl;
	for (const auto& entry : fs::directory_iterator(basePath)) {
		std::cout << "     " << entry.path().filename() << std::endl;
	}
	std::cout << "Enter model name: ";
	std::cin >> modelPath;
	std::string path = basePath + modelPath;
	std::cout << "Setting model in: " << path << std::endl;

	std::ofstream outFile(path, std::ios::binary);
	if (outFile) {
		size_t size = model.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

		// Write each point in the model
		outFile.write(reinterpret_cast<const char*>(model.data()), size * sizeof(p3));
	}
	else {
		std::cerr << "Error opening file for writing." << std::endl;
	}
	outFile.close();
}



// Function to read a model from a binary file
std::vector<p3> readModel() {
	std::string basePath = "Resources/Plain models/";
	std::string modelPath;
	std::cout << "set model path to one of these: " << std::endl;
	for (const auto& entry : fs::directory_iterator(basePath)) {
		std::cout << "     " << entry.path().filename() << std::endl;
	}
	std::cout << "Enter model name: ";
	std::cin >> modelPath;
	std::vector<p3> model;
	std::string path = basePath + modelPath;

	std::ifstream inFile(path, std::ios::binary);
	if (inFile) {
		size_t size;
		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		model.resize(size);
		inFile.read(reinterpret_cast<char*>(model.data()), size * sizeof(p3));
	}
	else {
		std::cerr << "Error opening file for reading." << std::endl;
	}
	inFile.close();

	return model;
}

std::vector<p3> readModel(const std::string modelPath) {
	std::string basePath = "Resources/Plain models/";

	std::vector<p3> model;
	std::string path = basePath + modelPath;

	std::ifstream inFile(path, std::ios::binary);
	if (inFile) {
		size_t size;
		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		model.resize(size);
		inFile.read(reinterpret_cast<char*>(model.data()), size * sizeof(p3));
	}
	else {
		std::cerr << "Error opening file for reading." << std::endl;
	}
	inFile.close();

	return model;
}

// Function to read ASCII STL file similar to your custom style
void readSTL(Polyhedra& stl, const std::string& filepath) {

	stl.clear();

	std::vector<p3>& positions = stl.positions;
	std::vector<p3>& normals = stl.normals;

	std::string basePath = "Resources/stl models/";
	std::ifstream inFile(basePath + filepath);



	if (!inFile) {
		std::cerr << "Unable to open file: " << basePath + filepath << std::endl;
		return;
	}

	std::string line;
	p3 normal, vertex;

	while (std::getline(inFile, line)) {
		std::istringstream iss(line);
		std::string word;
		iss >> word;

		if (word == "facet") {
			// Read normal vector
			iss >> word; // Skip "normal"
			iss >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
			normals.push_back(normal);
			normals.push_back(normal);
			//print(normal);
		}
		else if (word == "vertex") {
			// Read and store vertex position
			iss >> vertex.x >> vertex.y >> vertex.z;
			vector<p3> interm;
			interm.push_back(vertex);
			positions.insert(positions.end(), { interm[0].x,interm[0].y,interm[0].z });
			//print(vertex);
		}
	}

	inFile.close();

	stl.simpleIndices();
}



/// Split a string by a delimiter (e.g. ',') into a list of tokens
std::vector<std::string> split(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, delimiter)) {
		tokens.push_back(item);
	}
	return tokens;
}

/// Given a LINESTRING-like substring "1.0 2.0, 3.1 4.2, ..."
/// parse out each "lon lat" and convert to p2
std::vector<p2> parseLineStringCoords(const std::string& coordString) {
	std::vector<p2> points;
	// split by comma -> ["1.0 2.0", "3.1 4.2", ...]
	auto commaSeparated = split(coordString, ',');
	for (auto& cs : commaSeparated) {
		std::stringstream ss(cs);
		float lon, lat;
		ss >> lon >> lat;  // read "1.0 2.0" etc.
		points.push_back({ lon, lat });
	}
	return points;
}

void readWKT(/*std::vector<p2>& positions,*/ vector<vector<p2>> & final, const std::string& filepath)
{
	std::ifstream inFile(filepath);
	if (!inFile) {
		std::cerr << "Unable to open file: " << filepath << std::endl;
		return;
	}

	std::string line;
	while (std::getline(inFile, line)) {
		// For simplicity, check if we have LINESTRING or MULTILINESTRING
		// in that whole line (typical for CSV WKT lines).
		// If you have more columns in CSV, you'd parse out just the WKT column.

		if (line.find("LINESTRING") != std::string::npos) {
			// Example:  "...LINESTRING (1.51417 38.6936,1.49 38.70)..."
			// 1) find substring inside parentheses
			size_t start = line.find("(");
			size_t end = line.rfind(")");
			if (start != std::string::npos && end != std::string::npos && end > start) {
				// e.g. "1.51417 38.6936,1.49 38.70"
				std::string coords = line.substr(start + 1, end - (start + 1));

				// 2) parse coords into p2
				auto pts = parseLineStringCoords(coords);
				//positions.insert(positions.end(), pts.begin(), pts.end());
				final.push_back(pts);
			}
		}
		else if (line.find("MULTILINESTRING") != std::string::npos) {
			// Example: "...MULTILINESTRING ((x1 y1, x2 y2),(x3 y3, x4 y4))..."
			// We'll have an *outer* parentheses pair, then multiple sets inside.
			size_t start = line.find("((");
			size_t end = line.rfind("))");
			if (start != std::string::npos && end != std::string::npos && end > start) {
				// Inside is something like: "x1 y1, x2 y2),(x3 y3, x4 y4"
				std::string multi = line.substr(start + 2, end - (start + 2));

				// Each sub-linestring is in parentheses, so let's split by ")(" 
				// Easiest is to replace ")(" with some delimiter, e.g. '|', then split.

				// naive approach: replace ")(" with "|"
				// (In a robust parser, you'd do more careful scanning.)
				// We'll do it quickly:
				for (size_t pos = 0; pos + 1 < multi.size(); ) {
					if (multi[pos] == ')' && multi[pos + 1] == '(') {
						multi[pos] = '|';
						multi[pos + 1] = '|';
					}
					pos++;
				}

				// Now we can split on "||"
				auto subStrings = split(multi, '|');
				for (auto& sub : subStrings) {
					// sub might look like: "x1 y1, x2 y2, x3 y3"
					// trim extra parentheses if any:
					// e.g. if it starts with ')' or '('
					// (in practice, it might be already stripped, but let's be safe).
					// We can parse the substring as a single LINESTRING:
					auto pts = parseLineStringCoords(sub);
					//positions.insert(positions.end(), pts.begin(), pts.end());
					final.push_back(pts);
				}
			}
		}
		// else if there's "POLYGON" or "MULTIPOLYGON", you can handle similarly if needed
	}

	inFile.close();
}




// Function to write a 3D polyhedra without dividing its surfaces in polygons (positions, normals and indices)
void writeSimplePolyhedra(Polyhedra& stl, string modelPath) 
{

	std::vector<p3>& model = stl.positions;
	const std::vector<p3>& normals = stl.normals;
	const std::vector<unsigned int>& indices = stl.indices;


	std::string basePath = "Resources/Simple polyhedra/";
	//std::string modelPath;

	//// List available models in the directory
	//std::cout << "Current models: " << std::endl;
	//for (const auto& entry : fs::directory_iterator(basePath))
	//{
	//	std::cout << "     " << entry.path().filename() << std::endl;
	//}
	/*std::cout << "Enter model name: ";
	std::cin >> modelPath;*/
	std::string path = basePath + modelPath;
	std::cout << "Setting model in: " << path << std::endl;

	std::ofstream outFile(path, std::ios::binary);
	if (outFile)
	{
		//model
		size_t size = model.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outFile.write(reinterpret_cast<const char*>(model.data()), size * sizeof(p3));

		//normals
		size = normals.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outFile.write(reinterpret_cast<const char*>(normals.data()), size * sizeof(p3));

		//indices
		size = indices.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outFile.write(reinterpret_cast<const char*>(indices.data()), size * sizeof(unsigned int));
	}
	else
	{
		std::cerr << "Error opening file for writing." << std::endl;
	}
	outFile.close();
}






//I've made as an intermediate step to modify the csv data of the map, maybe delete it in the future?
//It's in a weird format, in my head it makes much more sense to store all positions sizes at the start and then all the positions
//, but I need to modify a lot of data, so here we are
void writeVectorOfVectors(const vector<vector<p2>>& model) {
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

void writeVectorOfVectorsAscii(const std::vector<std::vector<p2>>& model)
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



struct BinariesManager {

	struct IndexEntry {
		enum DataType { ProgramType, ModelPath };
		DataType type;
		size_t offset;
		size_t size;
	};
	/*for (const auto& entry : indexEntries) {
	std::cout << "Type: ";
	if (entry.type == IndexEntry::ProgramType) {
		std::cout << "ProgramType, ";
	}
	else if (entry.type == IndexEntry::ModelPath) {
		std::cout << "ModelPath, ";
	}
	std::cout << "Offset: " << entry.offset << ", Size: " << entry.size << std::endl;
	}
	cout << "MODELPATH " << modelPath << endl;*/

	std::string basePath = "Resources/";

	enum programType { CAD, RUNNING };

	//these two are read from config
	//programType currentProgramType = RUNNING;
	//std::string modelPath = "1-goliath.bin";
	programType currentProgramType;
	std::string modelPath;

	IndexEntry indexEntries[2];
	BinariesManager() {

		//writeConfig();
		readConfig();
	}


	//si cierras writeConfig sin terminar el proceso config desaparece, mira a ver
	void writeConfig() {
		system("cls");

		std::string input;

		bool foundModelPath = false;




		cout << "set currentProgramType { CAD, RUNNING }: ";
		std::cin >> input;
		if (input == "CAD" || input == "0")
		{
			currentProgramType = CAD;
		}
		else if (input == "RUNNING" || input == "1")
		{
			currentProgramType = RUNNING;
		}
		else
		{
			cout << "error invalid programType name" << endl;
			return;
		}



		if (currentProgramType == RUNNING)
		{
			std::string possibleModelPath;
			cout << "set model path to one of these: " << endl;
			for (const auto& entry : fs::directory_iterator(basePath)) {
				std::cout << "     " << entry.path().filename() << std::endl;
			}
			std::cin >> possibleModelPath;


			for (const auto& entry : fs::directory_iterator(basePath)) {
				if (possibleModelPath == entry.path().filename())
				{
					foundModelPath = true;
					break;
				}
			}
			if (foundModelPath)
			{

				modelPath = possibleModelPath;

				std::ofstream outFile("Resources/config.bin", std::ios::binary);
				if (outFile)
				{

					//now that we have the values of programType and modelPath, we set indexEntries and write it all
					indexEntries[0] = { IndexEntry::ProgramType, sizeof(indexEntries), sizeof(programType) };
					indexEntries[1] = { IndexEntry::ModelPath, sizeof(indexEntries) + sizeof(programType), modelPath.size() };

					for (const auto& entry : indexEntries)
					{
						outFile.write(reinterpret_cast<const char*>(&entry), sizeof(entry));
					}
					outFile.write(reinterpret_cast<const char*>(&currentProgramType), sizeof(currentProgramType));
					outFile.write(modelPath.data(), modelPath.size());

				}
				outFile.close();
			}
			else
				cout << "file not found" << endl;

		}
		else if (currentProgramType == CAD)
		{
			cout << "CAD" << endl;

			std::ofstream outFile("Resources/config.bin", std::ios::binary);
			if (outFile)
			{

				//now that we have the values of programType and modelPath, we set indexEntries and write it all
				indexEntries[0] = { IndexEntry::ProgramType, sizeof(indexEntries), sizeof(programType) };
				indexEntries[1] = { IndexEntry::ModelPath, sizeof(indexEntries) + sizeof(programType), modelPath.size() };

				for (const auto& entry : indexEntries)
				{
					outFile.write(reinterpret_cast<const char*>(&entry), sizeof(entry));
				}
				outFile.write(reinterpret_cast<const char*>(&currentProgramType), sizeof(currentProgramType));
			}
			outFile.close();

		}


	}

	void readConfig() {
		//system("cls");


		std::ifstream inFileConfig("Resources/config.bin", std::ios::binary);

		if (inFileConfig)
		{
			//sets all indexEntries
			inFileConfig.read(reinterpret_cast<char*>(indexEntries), sizeof(indexEntries));


			inFileConfig.seekg(indexEntries[0].offset);
			inFileConfig.read(reinterpret_cast<char*>(&currentProgramType), indexEntries[0].size);



			inFileConfig.seekg(indexEntries[1].offset);
			modelPath.resize(indexEntries[1].size);

			if (inFileConfig.read(&modelPath[0], modelPath.size())) {
				std::cout << endl << "modelPath: " << modelPath << std::endl << std::endl;
			}

		}
		else
		{
			std::cout << "config not found? This should not happen" << endl;
		}
		inFileConfig.close();

	}

	void writeModel(std::vector<float> model) {
		cout << "save you model, currently if the file the are these ones:" << endl;
		for (const auto& entry : fs::directory_iterator(basePath)) {
			std::cout << "     " << entry.path().filename() << std::endl;
		}
		std::cin >> modelPath;
		std::string path = basePath + modelPath;
		cout << "setting model in: " << path << endl;

		std::ofstream outFile(path, std::ios::binary);
		if (outFile)
		{
			size_t size = model.size();
			outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
			outFile.write(reinterpret_cast<const char*>(model.data()), size * sizeof(int));
		}
		outFile.close();

	}

	std::vector<float> readModel()
	{
		std::vector<float> model;
		std::string path = basePath + modelPath;

		std::ifstream inFile(path, std::ios::binary);

		if (inFile)
		{
			size_t size;
			inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
			model.resize(size);
			inFile.read(reinterpret_cast<char*>(model.data()), size * sizeof(int));

		}
		inFile.close();

		return model;
	}
};