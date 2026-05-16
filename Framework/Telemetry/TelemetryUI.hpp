#pragma once

#include "Telemetry.hpp"

#include "Axis.hpp"
#include "Plot.hpp"
#include "Overlay2D.hpp"
#include "ProgressBar.hpp"
#include "LourdesGraphic.hpp"


struct TelemetryUI
{
	Shader& shader3D;
	Shader& shader2D;
	Shader& shader2DInstanced;
	Shader& shaderText;
	Camera& camera;

	LourdesGraphic lourdesGraphic;

	Telemetry& telemetry;

	Lines2D centerCross;
	Overlay2D overlay;
	Plot graphic;
	Plot graphic2;
	ProgressBar pb;

	Axis axis;

	float pbValue = 0.5f;

	

	TelemetryUI(Telemetry& telemetry_, Shader& shader3D_, Shader& shader2D_, Shader& shader2DInstanced_, Shader& shaderText_, Camera& camera_)
		:telemetry(telemetry_)
		, shader3D(shader3D_), shader2D(shader2D_), shader2DInstanced(shader2DInstanced_), shaderText(shaderText_), camera(camera_)
		, axis(shader3D), lourdesGraphic(shader3D,camera)
		, overlay(shader2D, camera)
		, graphic(shader2D, shader2DInstanced, shaderText, camera, telemetry.tm, "A*cos(x)", { 1400,100 }, graf1Val)
		, graphic2(shader2D, shader2DInstanced, shaderText, camera, telemetry.tm, "rudderAngle", { 1400,400 }, lourdesGraphic.rudderAngle)
		, pb(shader2D, shader2DInstanced, shaderText, camera, telemetry.tm, p2{ 1350,700 }, "Battery", pbValue)
	{
		centerCross.addSet({
				{ centerWindow.x - 20, centerWindow.y},{ centerWindow.x + 20, centerWindow.y},
			{ centerWindow.x, centerWindow.y - 20},{ centerWindow.x, centerWindow.y + 20} });
		centerCross.indices = { 0,1,2,3 };
	}

	void draw()
	{
		axis.draw();

		shader3D.bind();
		shader3D.setUniform("u_Model", identityMatrix);
		shader3D.setUniform("u_fragmentMode", 1);

		lourdesGraphic.draw();

		opaque();

		shader2D.bind();

		overlay.draw();

		graf1Val = cosPlot(c);
		graphic.draw();
		graphic2.draw();
		pb.draw();

		transparent();
		shader2D.bind();
		shader2D.setUniform("u_Model", identityMatrix);
		shader2D.setUniform("u_Color", 1, 1, 1, 0.5);
		glLineWidth(2);
		centerCross.draw();
		glLineWidth(1);
		opaque();

		
	}
};