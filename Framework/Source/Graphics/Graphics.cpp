#include "Common.hpp"
#include "Graphics.hpp"

void initializeCameraLocations(Shader& shader3D, Shader& shader2D, Shader& shader2D_Instanced, Shader& shaderText, Camera& camera)
{
	//3D
	shader3D.bind();
	shader3D.setUniform("u_Perspective", camera.perspectiveMatrix);
	shader3D.setUniform("u_Model", identityMatrix);

	shader3D.setUniform("u_CamPos", camera.cameraPos);
	shader3D.setUniform("u_fragmentMode", flatColor);

	//2D
	shader2D.bind();
	shader2D.setUniform("u_OrthoProjection", camera.orthoMatrix);
	shader2D.setUniform("u_Model", identityMatrix);


	//2DInstanced
	shader2D_Instanced.bind();
	shader2D_Instanced.setUniform("u_OrthoProjection", camera.orthoMatrix);


	//Text
	shaderText.bind();
	shaderText.setUniform("u_OrthoProjection", camera.orthoMatrix);
	shaderText.setUniform("u_Color", 1.0f, 1.0f, 1.0f);
}

void updateCameraLocations(Shader& shader3D, Camera& camera)
{
	shader3D.bind();
	shader3D.setUniform("u_View", camera.viewMatrix);
}



void polyhedraToPolygon2D(Polyhedra polyhedra, Polygons2D& polygon)
{
	//removes the .y positions

	vector<p2> positions2D;

	for (auto& pos : polyhedra.positions)
	{
		positions2D.push_back({ pos.x,pos.z });
	}

	//same indices in the polygon
	polygon.addSet(positions2D, polyhedra.indices);
}