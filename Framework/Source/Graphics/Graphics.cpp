#include "Common.hpp"
#include "Graphics.hpp"

void initializeCameraLocations(Shader& shader3D, Shader& shader2D, Shader& shader2D_Instanced, Shader& shaderText, Shader& shaderText3D, Shader& shaderWater, Camera& camera)
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

	//Text3D
	shaderText3D.bind();
	shaderText3D.setUniform("u_Perspective", camera.perspectiveMatrix);
	shaderText3D.setUniform("u_Model", identityMatrix);
	shaderText3D.setUniform("u_Color", 1.0f, 1.0f, 1.0f);
	shaderText3D.setUniform("u_fragmentMode", flatColor);

	//Water
	shaderWater.bind();
	shaderWater.setUniform("u_Perspective", camera.perspectiveMatrix);
	shaderWater.setUniform("u_Model", identityMatrix);

	shaderWater.setUniform("u_CamPos", camera.cameraPos);
	shaderWater.setUniform("u_fragmentMode", flatColor);

}

void updateCameraLocations(Shader& shader3D, Shader& shaderText3D, Shader& shaderWater, Camera& camera)
{
	shader3D.bind();
	shader3D.setUniform("u_View", camera.viewMatrix);

	shaderText3D.bind();
	shaderText3D.setUniform("u_View", camera.viewMatrix);

	shaderWater.bind();
	shaderWater.setUniform("u_View", camera.viewMatrix);
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