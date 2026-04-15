#pragma once
#include "TelemetryUI.hpp"

struct Telemetry
{
	Shader& shader3D;
	Shader& shader2D;
	Shader& shader2D_Instanced;
	Shader& shaderText;
	Camera& camera;
	TimeStruct& tm;
	GlobalVariables& gv;
	

	Axis axis;

	Lourdes& lourdes;
	TelemetryUI ui;
	
	Telemetry(Shader& shader3D_, Shader& shader2D_, Shader& shader2D_Instanced_, Shader& shaderText_, Camera& camera_, GlobalVariables& gv_, TimeStruct& tm_, Lourdes& lourdes_)
		:shader3D(shader3D_), shader2D(shader2D_), shader2D_Instanced(shader2D_Instanced_), shaderText(shaderText_), camera(camera_), gv(gv_), tm(tm_), lourdes(lourdes_)
		, axis(shader3D, gv)
		,ui(shader3D, shader2D, shader2D_Instanced, shaderText,camera, gv,tm, lourdes)
		
	{

		
	}

	void update()
	{
		axis.draw();

		ui.draw();

		
	}
};