//Polyhedra works with full models that can be imported from other formats or created in Polygons3d

//here some functions that were inside of polyhedra to try to add them

//void addPositions(const vector<p3>& positions_, int mode = 0)
//{
//	//0 convex
//
//	positions.insert(positions.end(), positions_.begin(), positions_.end());
//	createConvexIndices(positions_);
//	calculateNormals();
//	isBufferUpdated = true;
//}
//
//void createConvexIndices(const vector<p3>& positions_)
//{
//	for (unsigned int i = 0; i < positions_.size() - 2; i++)
//	{
//		indices.insert(indices.end(), { 0, i + 1, i + 2 });
//
//	}
//
//}
//void calculateNormals()
//{
//	for (int i = 0; i < indices.size(); i += 3)
//	{
//		p3 a = positions[indices[i]];
//		p3 b = positions[indices[i + 1]];
//		p3 c = positions[indices[i + 2]];
//
//		normals.push_back(normalize3(cross3(b - a, c - a)));
//	}
//}




//#pragma once
//
//#include "Common.hpp"
//
//#include "SweepTriangulation.hpp"
//#include "Lines3D.hpp"
//
//
///////////////ESTO ESTÁ TURBO DESACTUALIZADO
///////////////DEBERÍA DE SER POLYGONS3D
//struct Polygons {
//	GLenum usageHint = GL_DYNAMIC_DRAW;
//
//	bool isBufferUpdated = false;
//
//
//	Lines3D lines;
//
//	//vector<p3> model;
//	vector<p3>& positions = lines.positions;
//	vector <unsigned int> indices; //triangle indices, do not mistake them with the lines indices
//	SweepTrAux aux; 
//
//	unsigned int vertexBuffer;
//	unsigned int vertexArray;
//	unsigned int indexBuffer;
//
//	///VECTOR??
//	p3 normal;
//
//	//Setting initial buffers size
//	size_t currentPositionsBufferSize = 0;
//	size_t currentIndicesBufferSize = 0;
//	size_t currentPositionsDataSize = 0;
//	size_t currentIndicesDataSize = 0;
//
//
//	
//
//	
//
//	float area = 0.0f;
//	p3 centroid;
//
//
//	Polygons() {
//		genBuffers();
//	}
//
//	~Polygons();
//
//	void genBuffers();
//
//	void draw();
//
//	void clear();
//
//	void addSet(vector<p3> items);
//
//	//Mira tío, he hecho amago por integrar esto, pero está muy sucio este file, prefiero usar polyhedra la verdad
//	//void addSetNew(vector<p3> items, int mode = 0)
//	//{
//
//	//}
//	//void createConvexIndices(const vector<p2>& items) {
//
//	//	for (unsigned int i = 0; i < items.size() - 3; i++)
//	//	{
//	//		indices.insert(indices.end(), { indexOffset,indexOffset + i + 1,indexOffset + i + 2 });
//
//	//	}
//
//	//	indexOffset = indices.back() + 2;//position[indices.back()+1] is positions[indices.front()], must be +2
//
//	//}
//
//	void calculateNormal() {
//		for (int i = 0; i < positions.size() - 1; i++)
//		{
//			normal = normalize3(cross3(positions[i + 1] - positions[i], positions[i + 2] - positions[i + 1]));
//			if (normal != p3{ 0, 0, 0 }) return;
//		}
//	}
//
//	
//
//
//
//
//
//	//void ConvexTriangulation();
//
//	void translate(p3 pos) {
//		for (auto& position : positions)
//		{
//			position += pos;
//		}
//		isBufferUpdated = true;
//	}
//	
//	void calculateArea() {
//		area = 0.0;
//
//		p3 normalSum = { 0, 0, 0 }; // Accumulator for cross products
//
//		
//		for (int i = 0; i < positions.size() - 1; ++i)
//		{
//			// Compute the cross product of consecutive edges and add to normalSum
//			normalSum += cross3(positions[i], positions[i + 1]);  // Assuming cross3 returns a p3
//		}
//
//		// Compute area as half the magnitude of the accumulated cross products
//		area = 0.5 * magnitude3(normalSum);  // Assuming magnitude3 returns a float
//	}
//
//	void centroidCalculation() {
//		if (area == 0) {
//			centroid = positions[0];
//			return;
//		}
//
//		centroid = { 0, 0, 0 }; // Initialize 3D centroid
//
//		for (int i = 0; i < positions.size() - 1; i++) {
//			// Calculate the cross product of consecutive edges to get the area contribution in 3D
//			p3 v1 = positions[i];
//			p3 v2 = positions[i + 1];
//			p3 crossProduct = {
//				v1.y * v2.z - v1.z * v2.y,
//				v1.z * v2.x - v1.x * v2.z,
//				v1.x * v2.y - v1.y * v2.x
//			};
//
//			// Use the area contribution (crossProduct) to accumulate centroid coordinates
//			float factor = (v1.x * v2.y - v2.x * v1.y);
//
//			centroid.x += (v1.x + v2.x) * factor;
//			centroid.y += (v1.y + v2.y) * factor;
//			centroid.z += (v1.z + v2.z) * factor;
//		}
//
//		// Scale by the total area
//		centroid.x *= (1 / (6.0 * area));
//		centroid.y *= (1 / (6.0 * area));
//		centroid.z *= (1 / (6.0 * area));
//	}
//};



//////////////////
//From Pyramid.hpp, to check if they have any value. Just delete it if not used by september 2025

//vertex data must be unique for each face (repeated positions)
//void calculateNormals1() {
//	normals.clear();
//	normals.resize(positions.size(), { 0,0,0 });
//
//	for (int i = 0; i < indices.size(); i += 3) {
//		vec3<float> pos1 = positions[indices[i]];
//		vec3<float> pos2 = positions[indices[i + 1]];
//		vec3<float> pos3 = positions[indices[i + 2]];
//
//		vec3<float> normal = normalize3(cross3(pos2 - pos1, pos3 - pos1));
//
//		normals[indices[i]] = normal;
//		normals[indices[i + 1]] = normal;
//		normals[indices[i + 2]] = normal;
//	}
//
//	// Normalize the normals
//	for (vec3<float>& normal : normals) {
//		normal = normalize3(normal);
//	}
//}

//normal averaging for shared vertices (unrepeated positions)
//void calculateNormals2() {
//	normals.clear();
//	normals.resize(positions.size(), { 0, 0, 0 });
//
//	// First, compute per-triangle normals and sum them to the corresponding vertex normals
//	for (int i = 0; i < indices.size(); i += 3) {
//		p3 pos1 = positions[indices[i]];
//		p3 pos2 = positions[indices[i + 1]];
//		p3 pos3 = positions[indices[i + 2]];
//
//		p3 normal = normalize3(cross3(pos2 - pos1, pos3 - pos1));
//
//		normals[indices[i]] += normal;
//		normals[indices[i + 1]] += normal;
//		normals[indices[i + 2]] += normal;
//	}
//
//	// Normalize all vertex normals
//	for (p3& normal : normals) {
//		normal = normalize3(normal);
//	}
//}
////not tested, to have sharp edges
//void calculateNormalsWithSharpEdges(float sharpAngleDegrees) {
//	normals.clear();
//	normals.resize(positions.size(), { 0, 0, 0 });
//
//	// Pre-calculate all face normals
//	std::vector<vec3<float>> faceNormals(indices.size() / 3);
//
//	for (int i = 0; i < indices.size(); i += 3) {
//		vec3<float> pos1 = positions[indices[i]];
//		vec3<float> pos2 = positions[indices[i + 1]];
//		vec3<float> pos3 = positions[indices[i + 2]];
//
//		vec3<float> faceNormal = normalize3(cross3(pos2 - pos1, pos3 - pos1));
//		faceNormals[i / 3] = faceNormal;
//	}
//
//	// Accumulate normals for each vertex but only if faces are smooth enough
//	float cosSharpAngleThreshold = cos(sharpAngleDegrees * PI / 180.0f); // Convert degrees to radians
//
//	for (int i = 0; i < indices.size(); i += 3) {
//		int idx1 = indices[i];
//		int idx2 = indices[i + 1];
//		int idx3 = indices[i + 2];
//
//		vec3<float> faceNormal = faceNormals[i / 3];
//
//		// For each edge, compare the normals of adjacent faces
//		addNormalIfSmooth(idx1, idx2, faceNormal, faceNormals, cosSharpAngleThreshold);
//		addNormalIfSmooth(idx2, idx3, faceNormal, faceNormals, cosSharpAngleThreshold);
//		addNormalIfSmooth(idx3, idx1, faceNormal, faceNormals, cosSharpAngleThreshold);
//	}
//
//	// Normalize the accumulated normals
//	for (vec3<float>& normal : normals) {
//		normal = normalize3(normal);
//	}
//}
//
//// Helper function to accumulate normal for a vertex if the adjacent face normals are smooth
//void addNormalIfSmooth(int vertexIndex1, int vertexIndex2, vec3<float>& faceNormal,
//	const std::vector<vec3<float>>& faceNormals, float cosThreshold) {
//	for (int i = 0; i < indices.size(); i += 3) {
//		// Check if vertexIndex1 and vertexIndex2 belong to another face
//		if ((indices[i] == vertexIndex1 && indices[i + 1] == vertexIndex2) ||
//			(indices[i] == vertexIndex2 && indices[i + 1] == vertexIndex1) ||
//			(indices[i] == vertexIndex1 && indices[i + 2] == vertexIndex2) ||
//			(indices[i] == vertexIndex2 && indices[i + 2] == vertexIndex1)) {
//
//			vec3<float> adjacentNormal = faceNormals[i / 3];
//			if (dot3(faceNormal, adjacentNormal) >= cosThreshold) {
//				normals[vertexIndex1] += faceNormal;
//				normals[vertexIndex2] += faceNormal;
//			}
//		}
//	}
//}