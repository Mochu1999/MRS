#pragma once
#include "Graphics.hpp"
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
	PlotTime plotSail;
	PlotTime plotRudder;
	ProgressBar pb;

	Axis axis;



	TelemetryUI(Telemetry& telemetry_, Shader& shader3D_, Shader& shader2D_, Shader& shader2DInstanced_, Shader& shaderText_, Camera& camera_)
		:t(telemetry_), shader3D(shader3D_), shader2D(shader2D_), shader2DInstanced(shader2DInstanced_), shaderText(shaderText_), camera(camera_)
	{
		pb.createPB(&t.battery, p2{ 1350,700 }, "Battery");
		plotSail.createPlot(&t.sailAngle, &t.tm.currentTime, { 1350,50 }, "sailAngle");
		plotRudder.createPlot(&t.rudderAngle,&t.tm.currentTime, { 1350,350 }, "rudderAngle");

		updateSunLocation();

		centerCross.addSet({
				{ centerWindow.x - 20, centerWindow.y},{ centerWindow.x + 20, centerWindow.y},
				{ centerWindow.x, centerWindow.y - 20},{ centerWindow.x, centerWindow.y + 20} });
		centerCross.indices = { 0,1,2,3 };

	}

	//structs that have more than one instance (here or in other project) have their own draw calls, otherwise they are here
	//THEY ALL WILL HAVE MULTIPLE INSTANCES ONCE SIMULATION PROJECT IS ON
	void draw()
	{
		axis.draw(shader3D);

		drawLourdes();
		drawWater();
		drawSun();

		overlay.draw(shader2D);
		plotSail.draw(shader2D, shader2DInstanced, shaderText);
		plotRudder.draw(shader2D, shader2DInstanced, shaderText);
		pb.draw(shader2D,shaderText);

		drawCenterCross();
	}



	void drawLourdes();

	void drawWater();

	void updateSunLocation();

	void drawSun();

	void drawCenterCross();


};