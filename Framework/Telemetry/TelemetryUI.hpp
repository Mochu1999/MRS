#pragma once

#include "Telemetry.hpp"

#include "Axis.hpp"
#include "Overlay2D.hpp"
#include "Plot.hpp"
#include "ProgressBar.hpp"
#include "Lourdes3DModel.hpp"

#include "WaterMRS.hpp"
#include "SunMRS.hpp"

struct TelemetryUI
{
	Shader& shader3D;
	Shader& shader2D;
	Shader& shader2DInstanced;
	Shader& shaderText;
	Camera& camera;

	Telemetry& t;

	Lourdes3DModel lourdesModel;
	WaterMRS water;
	SunMRS sun;

	Lines2D centerCross;
	Overlay2D overlay;
	Plot plot;
	Plot plot2;
	ProgressBar pb;

	Axis axis;

	float pbValue = 0.5f;

	float borrar;


	std::array<float, 16> shipModelMatrix = identityMatrix;
	std::array<float, 16> rudderMatrix, propellerMatrixf;




	TelemetryUI(Telemetry& telemetry_, Shader& shader3D_, Shader& shader2D_, Shader& shader2DInstanced_, Shader& shaderText_, Camera& camera_)
		:t(telemetry_)
		, shader3D(shader3D_), shader2D(shader2D_), shader2DInstanced(shader2DInstanced_), shaderText(shaderText_), camera(camera_)
		, plot(shader2D, shader2DInstanced, shaderText, camera, t.tm, "sailAngle", { 1400,100 }, t.sailAngle)
		, plot2(shader2D, shader2DInstanced, shaderText, camera, t.tm, "rudderAngle", { 1400,400 }, t.rudderAngle)
		, pb(shader2D, shader2DInstanced, shaderText, camera, t.tm, p2{ 1350,700 }, "Battery", pbValue)
	{
		updateSunLocation();

		centerCross.addSet({
				{ centerWindow.x - 20, centerWindow.y},{ centerWindow.x + 20, centerWindow.y},
				{ centerWindow.x, centerWindow.y - 20},{ centerWindow.x, centerWindow.y + 20} });
		centerCross.indices = { 0,1,2,3 };
	}

	void draw()
	{
		drawAxis();

		drawLourdes();
		drawWater();
		drawSun();

		drawOverlay();


		plot.draw();
		plot2.draw();
		pb.draw();



		drawCenterCross();
	}

	

	void drawLourdes();

	void drawAxis();

	void drawWater();

	void updateSunLocation();

	void drawSun();

	void drawCenterCross();

	void drawOverlay();
};