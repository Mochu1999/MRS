#pragma once
#include "Graphics.hpp"
#include "Telemetry.hpp"

#include "Route.hpp"
#include "Axis.hpp"
#include "Overlay2D.hpp"
#include "Plot.hpp"
#include "ProgressBar.hpp"
#include "Lourdes3DModel.hpp"

#include "AuxVisual3D.hpp"
#include "SunMRS.hpp"

#include "Buttons.hpp"
#include "FpsCounter.h"

#include "AuxLonLats.hpp"
#include "World.hpp"
#include "Icons.hpp"

struct DataRoute
{
	Telemetry& t;

	//box
	Polygons2D box;
	Lines2D boxOutline;
	Text boxText;

	//Route
	Polygons2D circleFinish;
	Polygons2D circlesNodes; //Nodes and ship's position
	Lines2D courseLine;


	DataRoute(Telemetry& t_) : t(t_)
	{
		boxText.createAtlas(36, "resources/Glyphs/Helvetica/Helvetica.otf");
		boxOutline.addSet(createRoundedSquare({ 80,370 }, 600, 380, 30));
		box.addSet(createRoundedSquare({ 80,370 }, 600, 380, 30));

		vector<p2> nodes = { t.position };
		nodes.push_back(t.finishPoint);
		circlesNodes.addCircle(2500, lonLatToMercator(nodes), 100);
		circleFinish.addCircle(5000, lonLatToMercator(t.finishPoint), 100);
		courseLine.addSet(lonLatToMercator(nodes));

	}

	//mapModel2DMatrix to scale the points down
	//THESE ELEMENTS SHOULD STAY OF A CONSTANT SIZE NO MATTER HOW MUCH YOU ZOOM IN OR OUT, BUT YOU STILL NEED TO KNOW THE TRANSLATION AND SCALE FACTORS OF THE MAP
	void draw(World& world, Shader& shader2D, Shader& shaderText)
	{
		shader2D.bind();
		shader2D.setUniform("u_Model", identityMatrix);
		shader2D.setUniform("u_Color", 40 / 255.0f, 40 / 255.0f, 40 / 255.0f, 1.0f);
		box.draw();
		shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
		glLineWidth(3);
		boxOutline.draw();
		glLineWidth(1);

		shaderText.bind();
		//ETA SHOULD USE AN AVERAGE shipSpeed
		float eta;
		if (magnitude2(t.shipSpeed) == 0) eta = std::numeric_limits<float>::quiet_NaN();
		else eta = round1d(t.totalDistance / magnitude2(t.shipSpeed) / 3600);


		boxText.addDynamicText({
			{{ 100,700 }, "Ship coordinates:  ", lonLatToString(t.position)},
			{ { 100,650 }, "Distance left:  ", round1d(t.totalDistance / 1000)," km"},
			{ { 100,600 }, "shipSpeed: ", round1d(meterSecondToKnot(magnitude2(t.shipSpeed))) ,"  knots"},
			{ { 100,550 }, "Estimated time left: ",eta," hours"},
			{ { 100,500 }, "Errors:  N/A"}
			});
		shaderText.setUniform("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		boxText.draw();

		shader2D.bind();

		matrix4x4 mapModel2DMatrix = identityMatrix;
		translate2DModelMatrix(mapModel2DMatrix, world.translationModel);
		scale2DModelMatrix(mapModel2DMatrix, world.scaleModel);

		shader2D.setUniform("u_Model", mapModel2DMatrix);

		shader2D.setUniform("u_Color", 0, 0, 1, 1.0f);
		courseLine.draw();

		shader2D.setUniform("u_Color", 1, 1, 1, 1);
		circlesNodes.draw();

		shader2D.setUniform("u_Color", 1.0f, 0, 0, 1.0f);
		circleFinish.draw();
	}

};


struct DataShip
{
	Overlay2D overlay;
	PlotTime plotSail;
	PlotTime plotRudder;
	ProgressBar pb;

	DataShip(Telemetry& t)
	{
		pb.createPB(&t.battery, p2{ 1350,700 }, "Battery");
		plotSail.createPlot(&t.sailAngle, &t.tm.currentTime, { 1350,50 }, "sailAngle");
		plotRudder.createPlot(&t.rudderAngle, &t.tm.currentTime, { 1350,350 }, "rudderAngle");

	}

	void draw(Shader& shader2D, Shader& shader2DInstanced, Shader& shaderText)
	{
		plotSail.update();
		plotRudder.update();

		overlay.draw(shader2D);
		plotSail.draw(shader2D, shader2DInstanced, shaderText);
		plotRudder.draw(shader2D, shader2DInstanced, shaderText);
		pb.draw(shader2D, shaderText);
	}

};

struct TelemetryUI
{
	Shader& shader3D;
	Shader& shader2D;
	Shader& shader2DInstanced;
	Shader& shaderText;
	Shader& shaderText3D;
	Shader& shaderWater;
	Camera& camera;

	Telemetry& t;

	Buttons& buttons;

	Lourdes3DModel lourdesModel;
	AuxVisual3D water;
	SunMRS sun;

	FpsCounter fpsCounter;

	//ship
	CenterCross centerCross;
	DataShip dataShip;
	Axis axis;

	//Route
	World world;
	DataRoute dataRoute;
	Ship2DIcon icon;


	TelemetryUI(Telemetry& telemetry_, Shader& shader3D_, Shader& shader2D_, Shader& shader2DInstanced_, Shader& shaderText_, Shader& shaderText3D_, Shader& shaderWater_, Camera& camera_, Buttons& buttons_)
		:t(telemetry_), shader3D(shader3D_), shader2D(shader2D_), shader2DInstanced(shader2DInstanced_), shaderText(shaderText_), shaderText3D(shaderText3D_), shaderWater(shaderWater_), camera(camera_), buttons(buttons_)
		, lourdesModel(t), fpsCounter(t.tm), dataRoute(t), dataShip(t), water(t)
	{
		//Ship
		

		sun.updateLightLocation(shader3D);
		sun.updateLightLocation(shaderWater);

		//Route
	}


	void draw()
	{
		if (programState == ship)
		{
			//axis.draw(shader3D);
			lourdesModel.draw(shader3D);
			water.draw(shader3D, shaderText3D,shaderWater);
			dataShip.draw(shader2D, shader2DInstanced, shaderText);
			sun.draw(shader3D);

			centerCross.draw(shader2D);
		}
		else if (programState == route)
		{
			world.draw(shader2D);
			dataRoute.draw(world, shader2D, shaderText);
			icon.draw(world, t, shader2D);
		}
		fpsCounter.draw(shaderText);

		buttons.draw(shader2D, shaderText);


	}

};