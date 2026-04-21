#include "Common.hpp"
#include "Graphics.hpp"

void cameraLocationsInitialization(Shader& shader3D, Shader& shader2D, Shader& shader2D_Instanced, Shader& shaderText, Camera& camera)
{
	//3D
	shader3D.bind();
	shader3D.setUniform("u_Perspective", camera.perspectiveMatrix);
	shader3D.setUniform("u_Model3D", identityMatrix);

	shader3D.setUniform("u_CamPos", camera.cameraPos);
	//Sun position for telemetry
	shader3D.setUniform("u_lightPos", p3{ 30,25,40 });

	//2D
	shader2D.bind();
	shader2D.setUniform("u_OrthoProjection", camera.orthoMatrix);
	shader2D.setUniform("u_Model", identityMatrix);


	//2D_Instanced
	shader2D_Instanced.bind();
	shader2D_Instanced.setUniform("u_OrthoProjection", camera.orthoMatrix);

	//Text
	shaderText.bind();
	shaderText.setUniform("u_OrthoProjection", camera.orthoMatrix);
}