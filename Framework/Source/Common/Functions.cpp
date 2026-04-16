#include "Functions.hpp"

void print_(const std::string& name, const matrix4x4& matrix)
{
	std::stringstream ss;
	ss << name << " = " << std::endl;

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			// For column-major order, access the matrix elements in this order:
			ss << matrix[col * 4 + row] << " ";
		}
		ss << std::endl; // Newline at the end of each row
	}

	std::cout << ss.str() << std::endl;
}


std::array<float, 4> createQuaternion(float angle, const p3& axis) {
	std::array<float, 4> q;
	float s = sin(angle / 2);
	q[0] = cos(angle / 2); // w
	q[1] = axis.x * s; // x
	q[2] = axis.y * s; // y
	q[3] = axis.z * s; // z
	return q;
}

std::array<float, 4> inverseQuaternion(const std::array<float, 4>& q) {
	return { q[0], -q[1], -q[2], -q[3] }; // q^-1 = [w, -x, -y, -z] for a unit quaternion
}

//Quaternion multiplication is non-commutative (order matters)
std::array<float, 4> multiplyQuaternions(const std::array<float, 4>& a, const std::array<float, 4>& b) {
	std::array<float, 4> result;
	result[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3]; // w
	result[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2]; // x
	result[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1]; // y
	result[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0]; // z
	return result;
}

// Normalizes the quaternion [w, x, y, z] in-place
void normalizeQuaternion(std::array<float, 4>& q)
{
	float mag = std::sqrt(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
	if (mag > 0.0f) {
		q[0] /= mag;
		q[1] /= mag;
		q[2] /= mag;
		q[3] /= mag;
	}
}

// q * p * q^-1
void rotatePoint(p3& point, const float& angle, const p3& axis) {
	std::array<float, 4> rotationQuaternion = createQuaternion(angle, axis);
	//p
	std::array<float, 4> pointQuat = { 0, point.x, point.y, point.z };
	std::array<float, 4> inverseQuat = inverseQuaternion(rotationQuaternion);


	std::array<float, 4> rotatedPointQuat = multiplyQuaternions(multiplyQuaternions(rotationQuaternion, pointQuat), inverseQuat);

	point = { rotatedPointQuat[1], rotatedPointQuat[2], rotatedPointQuat[3] };
}


void rotate3D(std::vector<p3>& vertices, const p3& centroid, float angleX, float angleY, float angleZ) {

	for (p3& vertex : vertices)
	{
		vertex -= centroid;
		rotatePoint(vertex, radians(angleX), { 1, 0, 0 });
		rotatePoint(vertex, radians(angleY), { 0, 1, 0 });
		rotatePoint(vertex, radians(angleZ), { 0, 0, 1 });
		vertex += centroid;
	}

}

void rotate3D(std::vector<p3>& vertices, float angleX, float angleY, float angleZ) {

	for (p3& vertex : vertices)
	{
		rotatePoint(vertex, radians(angleX), { 1, 0, 0 });
		rotatePoint(vertex, radians(angleY), { 0, 1, 0 });
		rotatePoint(vertex, radians(angleZ), { 0, 0, 1 });
	}

}






float isRightOfLine(p2& A, p2& B, p2& P) {		 //is P to the right of AB?
	p2 AB = { B.x - A.x,  B.y - A.y };
	p2 AP = { P.x - A.x, P.y - A.y };

	return AB.x * AP.y - AB.y * AP.x;	//if negative it is to its right, if 0, P is on the infinite line of AB
}


float isBelowTriangle(const p3& a, const p3& b, const p3& c, const p3& p) {

	//equivalent to the signed volume of a tetrahedron without the /6.0f
	return dot3(cross3(b - a, c - a), p - a);

}


//Computes the intersection between 2 edges AB and CD
bool calculateIntersectionPoints(const p2 A, const p2 B, const p2 C, const p2 D, p2& i) {
	//It assumes AB, CD are not 0	//what if they are?
	p2 AB = B - A;
	p2 CD = D - C;
	p2 AC = C - A;

	/*
	In 2d geometry the cross product is a scalar value, not a vector, it represents the area of their parallelogram
	when:
	t = 0: Intersection at point A.
	0 < t < 1: Intersection between A and B (on the line segment).
	t = 1: Intersection at point B.
	t < 0: Intersection lies before A
	t > 1: Intersection lies beyond B
	(so t is the percentage between A and B)
	*/
	float precalculate = cross2(AB, CD);
	if (!precalculate) { // They are parallel
		// Check if C is on the line segment AB
		float scalarProjection = dot2(AC, AB) / (AB.x * AB.x + AB.y * AB.y);
		float crossProduct = cross2(AC, AB);

		if (scalarProjection >= 0 && scalarProjection <= 1 && crossProduct == 0) {
			i = C;
			return true;
		}
		return false;
	}

	float t = cross2(AC, CD) / precalculate; // (AC×CD)/(AB×CD) //position along AB
	float u = cross2(AC, AB) / precalculate; // (AC×AB)/(AB×CD) //position along CD

	if (t >= 0 && t <= 1 && u >= 0 && u < 1) { // Return false for intersection in exactly D
		i.x = A.x + t * AB.x;
		i.y = A.y + t * AB.y;
		return true;
	}

	return false;
}



void stableQuadraticSolver(float A, float B, float C, float& s0, float& s1)
{
	//Discriminat of the quadratic equation (B^2-4AC) >= 0
	float discr = B * B - 4.0 * A * C;
	//There won't be solution without a positive discriminant
	if (discr >= 0.0)
	{
		//This is an algorithm to solve the quadratic equation that is more stable (not faster)
		//Apparently in ray tracing, when A>>sqrt(discr). It can become unstable, and the only given solution garbage
		//Ignore the logic of how the quadratic is solved, it's called 
		float sqrtD = sqrt(discr);
		float q = -0.5 * (B + copysign(sqrtD, B));

		if (fabs(q) > eps)
		{
			s0 = q / A;
			s1 = C / q;
		}
		else
		{
			s0 = (-B - sqrtD) / (2.0 * A);
			s1 = (-B + sqrtD) / (2.0 * A);
		}
	}
}