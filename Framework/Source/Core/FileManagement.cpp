#include "Common.hpp"
#include "FilesManagement.hpp"

void readSTL(const std::string& localPath
	, std::vector<p3>& positions, std::vector<p3>& normals, std::vector<unsigned int>& indices)
{
	std::string basePath = "Resources/stlModels/";
	std::ifstream inFile(basePath + localPath);

	if (!inFile)
	{
		std::cerr << "Unable to open file: " << basePath + localPath << std::endl;
		return;
	}

	std::string line;
	p3 normal, vertex;

	while (std::getline(inFile, line))
	{
		std::istringstream iss(line);
		std::string word;
		iss >> word;

		if (word == "facet")
		{
			// Reads normal vector
			iss >> word; // Skip "normal"
			iss >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
			normals.push_back(normal);
			normals.push_back(normal);
			//print(normal);
		}
		else if (word == "vertex")
		{
			// Reads position
			iss >> vertex.x >> vertex.y >> vertex.z;
			vector<p3> interm;
			interm.push_back(vertex);
			positions.insert(positions.end(), { interm[0].x,interm[0].y,interm[0].z });
			//print(vertex);
		}
	}
	//Triangles will go like: 0,1,2-3,4,5-6,7,8-...
	for (int i = 0; i < positions.size(); i++)
	{
		indices.push_back(i);
	}
	inFile.close();
}

void readSimplePolyhedra(const std::string& localPath
	, std::vector<p3>& positions, std::vector<p3>& normals, std::vector<unsigned int>& indices)
{
	std::string path = "Resources/SimplePolyhedra/" + localPath;

	std::ifstream inFile(path, std::ios::binary);
	if (inFile)
	{
		size_t size;

		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		positions.resize(size);
		inFile.read(reinterpret_cast<char*>(positions.data()), size * sizeof(p3));

		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		normals.resize(size);
		inFile.read(reinterpret_cast<char*>(normals.data()), size * sizeof(p3));

		inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		indices.resize(size);
		inFile.read(reinterpret_cast<char*>(indices.data()), size * sizeof(unsigned int));

		inFile.close();
	}
	else
	{
		std::cerr << "Error opening file for reading." << std::endl;
	}

}

void writeSimplePolyhedra(const std::string& localPath
	, std::vector<p3>& positions, std::vector<p3>& normals, std::vector<unsigned int>& indices)
{
	std::string basePath = "Resources/SimplePolyhedra/";

	std::string outputPath = basePath + localPath;
	std::cout << "Setting model in: " << outputPath << std::endl;
	std::ofstream outFile(outputPath, std::ios::binary);
	if (outFile)
	{
		//model
		size_t size = positions.size();
		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		outFile.write(reinterpret_cast<const char*>(positions.data()), size * sizeof(p3));

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

// ascii stls to custom SimplePolyhedra format
void stlToSimplePolyhedra(const std::string& inputLocalPath, const std::string& outputLocalPath)
{
	std::vector<p3> positions;
	std::vector<p3> normals;
	std::vector<unsigned int> indices;


	readSTL(inputLocalPath, positions, normals, indices);
	writeSimplePolyhedra(outputLocalPath, positions, normals, indices);
}
// stlToBinary("pendulum1.stl", "pendulum1.bin");
