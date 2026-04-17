#pragma once

//--- --- ---
//prints
//--- --- ---

// General print template for all types
template<typename T>
void print_(const string& name, const T& item) {
	stringstream ss;
	ss << name << ": " << item;
	cout << ss.str() << endl << endl;
}

// Overload for vec2<T>
template<typename T>
void print_(const string& name, const vec2<T>& items) {
	stringstream ss;
	ss << name << ": ";
	ss << "{" << items.x << "," << items.y << "}";
	cout << ss.str() << endl << endl;
}



//vector<vec2<T>>
template<typename T>
void print_(const string& name, const vector<vec2<T>>& items) {
	stringstream ss;
	ss << name << ": {";
	for (size_t i = 0; i < items.size(); ++i) {
		ss << "{" << items[i].x << "," << items[i].y << "}";

		if (i != items.size() - 1) { // Check if it's not the last element
			ss << ",";
		}
	}
	cout << ss.str() << "}" << endl << endl;
}

//vec3<T>
template<typename T>
void print_(const string& name, const vec3<T>& items) {
	stringstream ss;
	ss << name << ": ";
	ss << "{" << items.x << "," << items.y << "," << items.z << "}";

	cout << ss.str() << endl << endl;
}


//vector<vec3<T>>
template<typename T>
void print_(const string& name, const vector<vec3<T>>& items) {
	stringstream ss;
	ss << name << ": {";
	for (size_t i = 0; i < items.size(); ++i) {
		ss << "{" << items[i].x << "," << items[i].y << "," << items[i].z << "}";

		if (i != items.size() - 1) { // Check if it's not the last element
			ss << ",";
		}
	}
	cout << ss.str() << "}" << endl << endl;
}

//vector<T>
template<typename T>
void print_(const string& name, const vector<T>& items) {
	stringstream ss;
	ss << name << ": ";
	ss << "{";
	for (size_t i = 0; i < items.size(); i++) {
		ss << items[i];

		if (i != items.size() - 1) { // Check if it's not the last element
			ss << ",";
		}
	}
	ss << "}";
	cout << ss.str() << endl << endl;
}

//Armadillo
//#include <armadillo>
//inline void print_(const std::string& name, const arma::vec& v)
//{
//	std::stringstream ss;
//	ss << name << ": {";
//	for (arma::uword i = 0; i < v.n_elem; ++i)
//	{
//		ss << v(i);
//		if (i + 1 != v.n_elem) ss << ",";
//	}
//	ss << "}";
//	std::cout << ss.str() << "\n\n";
//}

//matrix4x4
void print_(const std::string& name, const std::array<float, 16>& matrix);

#define print(var) print_(#var, var)









//--- --- ---
//Quaternions
//--- --- ---

std::array<float, 4> createQuaternion(float angle, const p3& axis);

std::array<float, 4> inverseQuaternion(const std::array<float, 4>& q);

std::array<float, 4> multiplyQuaternions(const std::array<float, 4>& a, const std::array<float, 4>& b);

// q * p * q^-1
void rotatePoint(p3& point, const float& angle, const p3& axis);

void rotate3D(std::vector<p3>& vertices, const p3& centroid, float angleX, float angleY, float angleZ);
void rotate3D(std::vector<p3>& vertices, float angleX, float angleY, float angleZ);

// Normalizes the quaternion [w, x, y, z] in-place
void normalizeQuaternion(std::array<float, 4>& q);




//--- --- ---
//
//--- --- ---

float isRightOfLine(p2& A, p2& B, p2& P);

float isBelowTriangle(const p3& a, const p3& b, const p3& c, const p3& p);

//Computes the intersection between 2 edges AB and CD
bool calculateIntersectionPoints(const p2 A, const p2 B, const p2 C, const p2 D, p2& i);

//Numerically Stable Quadratic Formula
void stableQuadraticSolver(float A, float B, float C, float& s0, float& s1);