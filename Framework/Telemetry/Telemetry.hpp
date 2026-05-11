#pragma once
#include "UI.hpp"

struct Telemetry
{
	Shader& shader3D;
	Shader& shader2D;
	Shader& shader2DInstanced;
	Shader& shaderText;
	Camera& camera;
	TimeStruct& tm;
	

	Axis axis;

	LourdesGraphic& lourdesGraphic;
	UI ui;
	
	Telemetry(Shader& shader3D_, Shader& shader2D_, Shader& shader2DInstanced_, Shader& shaderText_, Camera& camera_, TimeStruct& tm_, LourdesGraphic& lourdesGraphic_)
		:shader3D(shader3D_), shader2D(shader2D_), shader2DInstanced(shader2DInstanced_), shaderText(shaderText_), camera(camera_), tm(tm_), lourdesGraphic(lourdesGraphic_)
		, axis(shader3D)
		,ui(shader3D, shader2D, shader2DInstanced, shaderText,camera,tm, lourdesGraphic)
		
	{

		
	}

	void update()
	{
		axis.draw();

		ui.draw();

		
	}
};