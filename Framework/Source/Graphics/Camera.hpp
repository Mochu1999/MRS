#pragma once

/*
matrices4x4 are column major
0  4  8  12
1  5  9  13
2  6  10 14
3  7  11 15
*/


struct Camera 
{
	std::array<float, 16> perspectiveMatrix, viewMatrix, vpMatrix, orthoMatrix;

	//Settings will overwrite these 2 values on read
	//Where the camera is and where it is looking at
	p3 cameraPos = { 10,10,0 };
	p3 forward = normalize3(p3{ -1,-1,0});
	p3 right, up;

	float fov = 60.0f * PI / 180;
	float aspectRatio = windowWidth / windowHeight;

	enum CameraModes { drag, FPS, centered };
	CameraModes cameraMode = centered;


	Camera()
	{
		orthoMatrix = createOrthoMatrix();
		perspectiveMatrix = createPerspectiveMatrix();

		updateCamera();
	}

	array<float, 16> createOrthoMatrix();

	array<float, 16> createPerspectiveMatrix();

	std::array<float, 16> createViewMatrix(const p3& right, const p3& up, p3 forward, const p3& cameraPos);
	
	//create a new forward vector from rotating the old one. In practice it will rotate around right and up
	void rotateForward(const float rotationAngle, const p3& rotationAxis);

	//creates all the direction vectors and updates the view and vp matrices
	void updateCamera();


};

//--- --- ---
// Creation of model matrices
//--- --- ---

//The order is always scale, rotate and translate
matrix4x4 create3DModelMatrix(const float scale, const float angleDeg, p3 axis, const p3 translation);

//These ones are to modify a given matrix
void scale3DModelMatrix(matrix4x4& model3DMatrix, const float scale);
void rotate3DModelMatrix(matrix4x4& model3DMatrix, const float angleDeg, p3 axis);
void translate3DModelMatrix(matrix4x4& model3DMatrix, const p3 translation);


matrix4x4 create2DModelMatrix(float scale, float angleDeg,const p2 translation);
matrix4x4 create2DModelMatrix(p2 scale, float angleDeg, const p2 translation);

void scale2DModelMatrix(matrix4x4& model2DMatrix, const float scale);
void scale2DModelMatrix(matrix4x4& model2DMatrix, const p2 scale);
void rotate2DModelMatrix(matrix4x4& model2DMatrix, const float angleDeg);
void translate2DModelMatrix(matrix4x4& model2DMatrix, const p2 translation);

