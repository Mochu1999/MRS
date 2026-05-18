#pragma once

//Custom types and their functions


// --- --- ---
//	  vec2
// --- --- ---
template<typename T>
struct vec2 {
	T x, y;

	vec2() : x(0), y(0) {} //initializing paramenters to 0 if not specified 

	vec2(T x, T y) : x(x), y(y) {} //normal constructor

	// Operator overloads for comparison
	bool operator == (const vec2& other) const {
		return x == other.x && y == other.y;
	}

	bool operator!=(const vec2& other) const {
		return !(*this == other);
	}

	// Operator overloads for arithmetic
	vec2 operator + (const vec2& other) const {
		return { x + other.x, y + other.y };
	}

	vec2 operator + (const T other) const { //PASSING WITHOUT REFERENCE MIGHT BE LESS EXPENSIVE, CAN YOU CHECK?
		return { x + other, y + other };
	}

	vec2 operator - (const vec2& other) const {
		return { x - other.x, y - other.y };
	}

	vec2 operator - (const T other) const { //PASSING WITHOUT REFERENCE MIGHT BE LESS EXPENSIVE, CAN YOU CHECK?
		return { x - other, y - other };
	}

	vec2 operator - () const {
		return { -x, -y };
	}

	vec2 operator *(T scalar) const {
		return { x * scalar, y * scalar };
	}

	vec2 operator *(vec2<T> other) const {
		return { x * other.x, y * other.y };
	}

	vec2 operator /(T scalar) const {
		return { x / scalar, y / scalar };
	}

	// Compound assignment operators
	vec2& operator += (const vec2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	vec2& operator -= (const vec2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	vec2& operator *= (T scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	vec2& operator /= (T scalar) {
		x /= scalar;
		y /= scalar;
		return *this;
	}
	bool operator < (const vec2& other) const {
		if (x == other.x) return y < other.y;
		return x < other.x;
	}

};

using p2 = vec2<float>;
using ui2 = vec2<unsigned int>;


//vec2 functions:

//sum of products, is also equal to |v1|*|v2|*cos(theta)
//when v1 and v2 are the same vector v, cos=0, and so it's |v|^2
//this is why when v1 and v2 are perpendicular the value is 0
template<typename T>
T dot2(const vec2<T>& v1, const vec2<T>& v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

//In 2d geometry the cross product is a scalar value, not a vector, it represents the area of their parallelogram
template<typename T>
T cross2(const vec2<T>& v1, const vec2<T>& v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

template<typename T>
float magnitude2(const vec2<T>& v) { //modulus
	float magnitudeSquared = v.x * v.x + v.y * v.y;
	return sqrt(magnitudeSquared);
}

template<typename T>
vec2<T> normalize2(const vec2<T>& v) {
	T magnitudeSquared = v.x * v.x + v.y * v.y;
	if (magnitudeSquared == 0.0f) {
		return vec2<T>(0, 0); // Return a zero vector to avoid division by zero
	}
	float magnitude_inv = fastInverseSqrt(magnitudeSquared);
	return vec2<T>(v.x * magnitude_inv, v.y * magnitude_inv);
}

//needed for the unordered_maps that take p as keys, it could be inside vec2, but this is more idiomatic
//umaps have average looking times of O(1), compared with O(logn) of map
struct p_hash {
	std::size_t operator()(const p2& point) const {
		return std::hash<float>()(point.x) ^ (std::hash<float>()(point.y) << 1);
	}
};


//there are a lot of hash functions
struct p_HashMultiplicative {
	std::size_t operator()(const p2& point) const {
		constexpr std::uint64_t k = 0x9ddfea08eb382d69ULL;
		std::uint64_t a, b;

		std::memcpy(&a, &point.x, sizeof(std::uint64_t));
		std::memcpy(&b, &point.y, sizeof(std::uint64_t));

		return (a * k) ^ (b * k);
	}
};
//for key std::pair<p2,p2>
struct pair_hash_multiplicative {
	std::size_t operator()(const std::pair<p2, p2>& edge) const {
		p_HashMultiplicative ph;
		std::size_t h1 = ph(edge.first);
		std::size_t h2 = ph(edge.second);

		// Combine the two hashes
		return h1 ^ (h2 * 0x9e3779b97f4a7c15ULL); // Using another large constant
	}
};






// --- --- ---
//	  vec3
// --- --- ---
template<typename T>
struct vec3 {
	T x, y, z;

	vec3() : x(0), y(0), z(0) {} //initializing paramenters to 0 if not specified 

	vec3(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {} //normal constructor

	// operator overloads
	bool operator == (const vec3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(const vec3& other) const {
		return !(*this == other);
	}

	vec3 operator + (const vec3& other) const {
		return { x + other.x, y + other.y, z + other.z };
	}

	vec3 operator - (const vec3& other) const {
		return { x - other.x, y - other.y, z - other.z };
	}

	vec3 operator-() const {
		return { -x, -y, -z };
	}

	vec3 operator *(T scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}

	vec3 operator /(T scalar) const {
		return { x / scalar, y / scalar, z / scalar };
	}

	vec3& operator += (const vec3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	vec3& operator += (T scalar) {
		x += scalar;
		y += scalar;
		z += scalar;
		return *this;
	}

	vec3& operator -= (const vec3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	vec3& operator *= (T scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	vec3& operator /= (T scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}
};

using p3 = vec3<float>;
using ui3 = vec3<unsigned int>;


//to do scalar * vec3 appart from vec3 * scalar
template<typename T>
vec3<T> operator*(T scalar, const vec3<T>& v) {
	return { v.x * scalar, v.y * scalar, v.z * scalar };
}

//sum of products
template<typename T>
T dot3(const vec3<T>& v1, const vec3<T>& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template<typename T>
vec3<T> cross3(const vec3<T>& v1, const vec3<T>& v2) {
	return vec3<T>(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

template<typename T>
vec3<T> normalize3(const vec3<T>& v) {
	T magnitudeSquared = v.x * v.x + v.y * v.y + v.z * v.z;
	if (magnitudeSquared == 0.0f) {
		return vec3<T>(0, 0, 0); // Return a zero vector to avoid division by zero
	}
	float magnitude_inv = fastInverseSqrt(magnitudeSquared);
	return vec3<T>(v.x * magnitude_inv, v.y * magnitude_inv, v.z * magnitude_inv);
}


//modulus - length
template<typename T>
float magnitude3(const vec3<T>& v) {
	float magnitudeSquared = v.x * v.x + v.y * v.y + v.z * v.z;
	return sqrt(magnitudeSquared);
}

template<typename T>
vec3<T> centroid(const vec3<T>& p1, const vec3<T>& p2, const vec3<T>& p3) {
	return (p1 + p2 + p3) * inv3;
}

template<typename T>
vec3<T> normal(const vec3<T>& p1, const vec3<T>& p2, const vec3<T>& p3) {
	return normalize3(cross3(p2 - p1, p3 - p1));
}


// --- --- ---
//  matrix4x4
// --- --- ---
using matrix4x4 = std::array<float, 16>;

matrix4x4 multiplyMatrices(const matrix4x4& a, const matrix4x4& b);
matrix4x4 invertMatrix(const matrix4x4& m);

std::array<float, 4> multiplyMatVec(const matrix4x4& m, const std::array<float, 4>& v);


// --- --- ---
//  Interface
// --- --- ---

inline p3 p2ToP3Y(p2& coords2D)
{
	return { coords2D.x,0,coords2D.y };
}
inline vector<p3> p2ToP3Y(vector<p2>& coords2D)
{
	vector<p3> coords3D;
	for (auto& coord : coords2D)
	{
		coords3D.push_back({ coord.x,0,coord.y });
	}
	return coords3D;
}