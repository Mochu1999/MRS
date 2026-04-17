#include "Common.hpp"
#include "Types.hpp"



matrix4x4 multiplyMatrices(const matrix4x4& a, const matrix4x4& b)
{
	matrix4x4 output = {};

	for (int i = 0; i < 4; ++i) { // row index
		for (int j = 0; j < 4; ++j) { // column index
			output[j * 4 + i] =
				a[0 * 4 + i] * b[j * 4 + 0] +
				a[1 * 4 + i] * b[j * 4 + 1] +
				a[2 * 4 + i] * b[j * 4 + 2] +
				a[3 * 4 + i] * b[j * 4 + 3];
		}
	}

	return output;
}

//Mat4x4 · vec4 = vec4
std::array<float, 4> multiplyMatVec(const matrix4x4& m, const std::array<float, 4>& v) {
	std::array<float, 4> output = {};

	for (int i = 0; i < 4; ++i) { // row index
		output[i] =
			m[0 * 4 + i] * v[0] +
			m[1 * 4 + i] * v[1] +
			m[2 * 4 + i] * v[2] +
			m[3 * 4 + i] * v[3];
	}

	return output;
}


matrix4x4 invertMatrix(const matrix4x4& m)
{
	// Create augmented 4×8 matrix  [ M | I ]
	float aug[4][8] = {};
	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
			aug[row][col] = m[col * 4 + row];          // column‑major access

		aug[row][4 + row] = 1.0f;                      // Right‑hand identity
	}

	// Gauss‑Jordan elimination with partial pivoting
	for (int col = 0; col < 4; ++col)
	{
		// Pivot: find the row with largest absolute value in this column
		int pivot = col;
		float maxAbs = std::fabs(aug[pivot][col]);
		for (int r = col + 1; r < 4; ++r)
		{
			float absVal = std::fabs(aug[r][col]);
			if (absVal > maxAbs) { maxAbs = absVal; pivot = r; }
		}
		if (maxAbs < 1e-8f)   // determinant is zero
			throw std::runtime_error("invertMatrix: singular matrix");

		// Swap current row with pivot row
		if (pivot != col)
			for (int c = 0; c < 8; ++c)
				std::swap(aug[col][c], aug[pivot][c]);

		// Scale pivot row to make pivot element = 1
		float invPivot = 1.0f / aug[col][col];
		for (int c = 0; c < 8; ++c)
			aug[col][c] *= invPivot;

		// Eliminate this column from the other rows
		for (int r = 0; r < 4; ++r)
		{
			if (r == col) continue;
			float factor = aug[r][col];
			for (int c = 0; c < 8; ++c)
				aug[r][c] -= factor * aug[col][c];
		}
	}

	// Extract the right‑hand 4×4 block (the inverse)
	matrix4x4 inv{};
	for (int row = 0; row < 4; ++row)
		for (int col = 0; col < 4; ++col)
			inv[col * 4 + row] = aug[row][4 + col];    // back to column‑major

	return inv;
}
