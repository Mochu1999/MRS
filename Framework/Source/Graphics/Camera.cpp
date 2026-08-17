#include "Graphics.hpp"
#include "Camera.hpp"




array<float, 16> Camera::createOrthoMatrix() {
	array<float, 16> orthoMatrix = {};

	float left = 0.0f;
	float right = windowWidth;
	float bottom = 0.0f;
	float top = windowHeight;
	float nearZ = -1.0f;
	float farZ = 1.0f;

	orthoMatrix[0] = 2.0f / (right - left);
	orthoMatrix[5] = 2.0f / (top - bottom);
	orthoMatrix[10] = -2.0f / (farZ - nearZ);
	orthoMatrix[12] = -(right + left) / (right - left);
	orthoMatrix[13] = -(top + bottom) / (top - bottom);
	orthoMatrix[14] = -(farZ + nearZ) / (farZ - nearZ);
	orthoMatrix[15] = 1.0f;

	return orthoMatrix;
}

array<float, 16> Camera::createPerspectiveMatrix()
{
	float nearZ = 0.1f;
	float farZ = 500000.0f;

	array<float, 16> perspectiveMatrix = {};

	float tanHalfFov = tan(fov / 2);

	perspectiveMatrix[0] = 1 / (aspectRatio * tanHalfFov);
	perspectiveMatrix[5] = 1 / (tanHalfFov);
	perspectiveMatrix[10] = -(farZ + nearZ) / (farZ - nearZ);
	perspectiveMatrix[11] = -1.0f;
	perspectiveMatrix[14] = -2 * farZ * nearZ / (farZ - nearZ);

	return perspectiveMatrix;
}

//we want to move the world so the camera frustrum ends clipped in the square of range -1,1 for the axes x and y; and same range for the
// near and far z planes
std::array<float, 16> Camera::createViewMatrix(const p3& right, const p3& up, p3 forward, const p3& cameraPos) {



	std::array<float, 16> viewMatrix = { 0 };

	//rotation: change of basis from the world axes to one aligned with the camera local axes right, up and forward respectlively
	//the forward element is negative because we consider -z to be forward
	viewMatrix[0] = right.x;
	viewMatrix[1] = up.x;
	viewMatrix[2] = -forward.x;

	viewMatrix[4] = right.y;
	viewMatrix[5] = up.y;
	viewMatrix[6] = -forward.y;

	viewMatrix[8] = right.z;
	viewMatrix[9] = up.z;
	viewMatrix[10] = -forward.z;

	//Translation. Moves the world en the opposite direction of the camera position so it ends up in the origin
	// It must be done with a dot product. Because otherwise when the camera's rotated, you wouldn't end in the origin because the camera
	// would be translated with respect of its local axes
	viewMatrix[12] = -dot3(right, cameraPos);
	viewMatrix[13] = -dot3(up, cameraPos);
	viewMatrix[14] = dot3(forward, cameraPos);


	viewMatrix[15] = 1.0f;

	return viewMatrix;
}









void Camera::rotateForward(const float rotationAngle, const p3& rotationAxis)
{
	p3 intermForward = forward;

	rotatePoint(intermForward, rotationAngle, rotationAxis); //quaternion method
	intermForward = normalize3(intermForward);

	//This is a very complex problem, making forward to overpass 90º and this logic just a simple workaround
	//Because pitch can be over 90 in a valid state, maybe to check that and to see if the 90º mark is passed at any moment
	//if (abs(intermForward.y) < 0.995)
	if (rotationAxis==right && ((intermForward.x<0)!=(forward.x<0)) && abs(intermForward.y) > 0.99)
	{
		
	}
	else
	{
		/*if (abs(intermForward.y) < 0.999)*/
		{
			forward = intermForward;
			right = normalize3(cross3(forward, { 0,1,0 }));
			up = cross3(right, forward);
		}
	}
}


void Camera::updateCamera()
{
	viewMatrix = createViewMatrix(right, up, forward, cameraPos);

	vpMatrix = multiplyMatrices(perspectiveMatrix, viewMatrix);
}






matrix4x4 create3DModelMatrix(const float scale, const float angleDeg, p3 axis, const p3 translation)
{
	matrix4x4 model3DMatrix = identityMatrix;

	scale3DModelMatrix(model3DMatrix, scale);
	rotate3DModelMatrix(model3DMatrix, angleDeg, axis);
	translate3DModelMatrix(model3DMatrix, translation);

	return model3DMatrix;
}







//first 3 elements of the diagonal. scale is a float, so same scale in the 3 dimensions
void scale3DModelMatrix(matrix4x4& model3DMatrix, const float scale)
{
	matrix4x4 scaleMatrix = identityMatrix;

	scaleMatrix[0] *= scale;
	scaleMatrix[5] *= scale;
	scaleMatrix[10] *= scale;

	model3DMatrix = multiplyMatrices(model3DMatrix, scaleMatrix);
}


void rotate3DModelMatrix(matrix4x4& model3DMatrix, const float angleDeg, p3 axis)
{
	//We don't operate directly over the model matrix because it would overwrite scaling
	// At the same time we must take into account that rotations accumulate
	matrix4x4 rotationMatrix = identityMatrix;

	float theta = radians(angleDeg);

	axis = normalize3(axis);
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	float c = std::cos(theta);
	float s = std::sin(theta);
	float oneMinusC = 1.0f - c;

	// Rodrigues rotation formula
	//r_xx = x*x*(1-c)+c, r_xy = x*y*(1-c)-z*s, r_xz = x*z*(1-c)+y*s
	//r_yx = y*x*(1-c)+z*s, r_yy = y*y*(1-c)+c, r_yz = y*z*(1-c)-x*s
	//r_zx = z*x*(1-c)-y*s, r_zy = z*y*(1-c)+x*s, r_zz = z*z*(1-c)+c


	// Column 0
	rotationMatrix[0] = x * x * oneMinusC + c; // r_xx
	rotationMatrix[1] = y * x * oneMinusC + z * s; // r_yx
	rotationMatrix[2] = z * x * oneMinusC - y * s; // r_zx

	// Column 1
	rotationMatrix[4] = x * y * oneMinusC - z * s; // r_xy
	rotationMatrix[5] = y * y * oneMinusC + c; // r_yy
	rotationMatrix[6] = z * y * oneMinusC + x * s; // r_zy

	// Column 2
	rotationMatrix[8] = x * z * oneMinusC + y * s; // r_xz
	rotationMatrix[9] = y * z * oneMinusC - x * s; // r_yz
	rotationMatrix[10] = z * z * oneMinusC + c; // r_zz

	model3DMatrix = multiplyMatrices(model3DMatrix, rotationMatrix);
}

//Fourth column
// Translation must be multiplied so it composes correctly with other transformations
void translate3DModelMatrix(matrix4x4& model3DMatrix, p3 t)
{
	matrix4x4 translationMatrix = identityMatrix;

	translationMatrix[12] = t.x;
	translationMatrix[13] = t.y;
	translationMatrix[14] = t.z;

	model3DMatrix = multiplyMatrices(model3DMatrix, translationMatrix);
}

//THESE 2 FUNCTIONS SHOULD USE THE ONES FROM BELLOW
matrix4x4 create2DModelMatrix(float scale, float angleDeg, const p2 translation)
{
	matrix4x4 modelMatrix = identityMatrix;

	float theta = radians(angleDeg);
	float c = std::cos(theta);
	float s = std::sin(theta);

	// Column 0: [scale * cosθ, scale * sinθ, 0, 0]
	modelMatrix[0] = scale * c;
	modelMatrix[1] = scale * s; //scale matrix values [1] and [4] stay 0, but they change to this after multiplication

	// Column 1: [-scale * sinθ, scale * cosθ, 0, 0]
	modelMatrix[4] = -scale * s;
	modelMatrix[5] = scale * c;

	// Column 2: [0, 0, 1, 0] 

	// Column 3: [translation.x, translation.y, 0, 1]
	modelMatrix[12] = translation.x;
	modelMatrix[13] = translation.y;

	return modelMatrix;
}
matrix4x4 create2DModelMatrix(p2 scale, float angleDeg, const p2 translation)
{
	matrix4x4 modelMatrix = identityMatrix;
	float theta = radians(angleDeg);
	float c = std::cos(theta);
	float s = std::sin(theta);


	// Column 0: [scale * cosθ, scale * sinθ, 0, 0]
	modelMatrix[0] = scale.x * c;
	modelMatrix[1] = scale.x * s;

	// Column 1: [-scale * sinθ, scale * cosθ, 0, 0]
	modelMatrix[4] = -scale.y * s;
	modelMatrix[5] = scale.y * c;

	// Column 2: [0, 0, 1, 0] 

	// Column 3: [translation.x, translation.y, 0, 1]
	modelMatrix[12] = translation.x;
	modelMatrix[13] = translation.y;

	return modelMatrix;
}





void scale2DModelMatrix(matrix4x4& model2DMatrix, const float scale)
{
	matrix4x4 scaleMatrix = identityMatrix;

	scaleMatrix[0] *= scale;
	scaleMatrix[1] *= scale;

	scaleMatrix[4] *= -scale;
	scaleMatrix[5] *= scale;

	model2DMatrix = multiplyMatrices(model2DMatrix, scaleMatrix);
}

void scale2DModelMatrix(matrix4x4& model2DMatrix, const p2 scale)
{
	matrix4x4 scaleMatrix = identityMatrix;
	scaleMatrix[0] *= scale.x;
	scaleMatrix[1] *= scale.x;

	scaleMatrix[4] *= scale.y;
	scaleMatrix[5] *= scale.y;

	model2DMatrix = multiplyMatrices(model2DMatrix, scaleMatrix);
}

//void rotate2DModelMatrix(matrix4x4& model2DMatrix, const float angleDeg)
//{
//	matrix4x4 rotationMatrix = identityMatrix;
//
//	float theta = radians(angleDeg);
//	float c = std::cos(theta);
//	float s = std::sin(theta);
//
//	rotationMatrix[0] *= c;
//	rotationMatrix[1] *= s;
//
//	rotationMatrix[4] *= -s;
//	rotationMatrix[5] *= c;
//
//	model2DMatrix = multiplyMatrices(model2DMatrix, rotationMatrix);
//}
void rotate2DModelMatrix(matrix4x4& model2DMatrix, const float angleDeg)
{
	matrix4x4 rotationMatrix = identityMatrix;

	float theta = radians(angleDeg);
	float c = std::cos(theta);
	float s = std::sin(theta);

	rotationMatrix[0] = c;
	rotationMatrix[1] = s;

	rotationMatrix[4] = -s;
	rotationMatrix[5] = c;

	model2DMatrix = multiplyMatrices(model2DMatrix, rotationMatrix);
}

void translate2DModelMatrix(matrix4x4& model2DMatrix, const p2 translation)
{
	matrix4x4 translationMatrix = identityMatrix;

	translationMatrix[12] += translation.x;
	translationMatrix[13] += translation.y;

	model2DMatrix = multiplyMatrices(model2DMatrix, translationMatrix);
}














