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
	Plot plot;
	/*Plot plot2;*/
	PlotTime plot2;
	ProgressBar pb;

	Axis axis;

	float pbValue = 0.5f;

	float borrar;






	TelemetryUI(Telemetry& telemetry_, Shader& shader3D_, Shader& shader2D_, Shader& shader2DInstanced_, Shader& shaderText_, Camera& camera_)
		:t(telemetry_)
		, shader3D(shader3D_), shader2D(shader2D_), shader2DInstanced(shader2DInstanced_), shaderText(shaderText_), camera(camera_)
		, plot(shader2D, shader2DInstanced, shaderText, camera, t.tm, "sailAngle", { 1400,100 }, t.sailAngle)
		/*, plot2(shader2D, shader2DInstanced, shaderText, camera, t.tm, "rudderAngle", { 1400,400 }, t.rudderAngle)*/
	{
		pb.createPB(&t.battery, p2{ 1350,700 }, "Battery");
		plot2.createPlot(&t.sailAngle, { 1400,400 }, "sailAngle");

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

		drawProgressBar();
		drawPlot();
		plot.draw();
		/*plot2.draw();*/


		drawCenterCross();
	}



	void drawLourdes();

	void drawAxis();

	void drawWater();

	void updateSunLocation();

	void drawSun();

	void drawCenterCross();

	void drawOverlay();

	void drawProgressBar()
	{
		shader2D.bind();
		shader2DInstanced.setUniform("u_Model", identityMatrix);


		//outer line
		shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
		glLineWidth(3);
		pb.outerRoundedSquare.draw();
		glLineWidth(1);

		//inner polygon
		shader2D.setUniform("u_Color", 0.7, 0.7, 0.7, 1);
		glEnable(GL_SCISSOR_TEST); //method to cut an object just for the rendering
		glScissor(pb.outerCorner.x - 10, 0, 10 + pb.outerLength.x * (*pb.percentage), windowHeight);
		pb.innerRoundedSquare.draw();
		glDisable(GL_SCISSOR_TEST);

		pb.textUpdate();

		shaderText.bind();
		pb.text.draw();
	}

	void drawPlot()
	{
		//Frame
		transparent();
		shader2D.bind();
		shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
		glLineWidth(3);
		shader2DInstanced.setUniform("u_Model", identityMatrix);
		plot2.frame.draw();
		glLineWidth(1);


		//Axis
		shader2DInstanced.bind();
		shader2DInstanced.setUniform("u_Model", identityMatrix);
		shader2D.setUniform("u_Color", 1, 1, 1, 1);
		plot2.axis.draw();

		/*
		graphicModel2DMatrix = create2DModelMatrix(axisCorner, 0, { gridWidth,gridHeight });
		shader2DInstanced.setUniform("u_Model", graphicModel2DMatrix);

		


		shader2DInstanced.setUniform("u_Color", 0.5, 0.5, 0.5, 0.5);
		gridDynamic.draw();

		shader2DInstanced.setUniform("u_Color", 1, 0, 0, 1);

		currentXgrid.addInstances({
			{  { (data.positions.back().y - minDataY) * scaleY,1 },radians(90) , { axisCorner.x - maxDataX + currentX,axisCorner.y}},
			{ { currentX - maxDataX,1 },0,{axisCorner.x,axisCorner.y + (data.positions.back().y - minDataY) * scaleY}}
			});
		currentXgrid.draw();
		*/

	}
};