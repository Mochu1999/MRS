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

#include "Buttons.hpp"

struct TelemetryUI
{
	Shader& shader3D;
	Shader& shader2D;
	Shader& shader2DInstanced;
	Shader& shaderText;
	Camera& camera;

	Buttons& buttons;

	Telemetry& t;

	Lourdes3DModel lourdesModel;
	WaterMRS water;
	SunMRS sun;

	CenterCross centerCross;
	Overlay2D overlay;
	PlotTime plotSail;
	PlotTime plotRudder;
	ProgressBar pb;

	Axis axis;

	Text test;

	TelemetryUI(Telemetry& telemetry_, Shader& shader3D_, Shader& shader2D_, Shader& shader2DInstanced_, Shader& shaderText_, Camera& camera_, Buttons& buttons_)
		:t(telemetry_), shader3D(shader3D_), shader2D(shader2D_), shader2DInstanced(shader2DInstanced_), shaderText(shaderText_), camera(camera_), buttons(buttons_)
	{
		pb.createPB(&t.battery, p2{ 1350,700 }, "Batterysn");
		plotSail.createPlot(&t.sailAngle, &t.tm.currentTime, { 1350,50 }, "sailAnglesssssss");
		plotRudder.createPlot(&t.rudderAngle,&t.tm.currentTime, { 1350,350 }, "rudderAnglessssssss");

		test.createAtlas(50);
		

		sun.updateLightLocation(shader3D);
	}


	void draw()
	{
		axis.draw(shader3D);
		

		lourdesModel.draw(shader3D, t);
		water.draw(shader3D);
		sun.draw(shader3D);

		shaderText.bind();
		transparent();
		TextEntry algo({ 100,500 }, "qweorñinqrgxvzbmADSGBCVBXGFJ");
		test.addDynamicText({ algo });
		test.draw();

		overlay.draw(shader2D);
		plotSail.draw(shader2D, shader2DInstanced, shaderText);
		plotRudder.draw(shader2D, shader2DInstanced, shaderText);
		pb.draw(shader2D,shaderText);

		buttons.draw(shader2D,shaderText);

		centerCross.draw(shader2D);
	}
};