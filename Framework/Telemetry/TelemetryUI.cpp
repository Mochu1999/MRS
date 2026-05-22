#include "TelemetryUI.hpp"
#include "Graphics.hpp"
#include "Telemetry.hpp"

#include "Axis.hpp"
#include "Overlay2D.hpp"
#include "Plot.hpp"
#include "ProgressBar.hpp"
#include "Lourdes3DModel.hpp"

#include "WaterMRS.hpp"
#include "SunMRS.hpp"
#include "Time.hpp"


void TelemetryUI::drawLourdes()
{
	std::array<float, 16> shipModelMatrix = identityMatrix;


	shader3D.bind();
	opaque();
	shader3D.setUniform("u_fragmentMode", shadeColor);


	
	translate3DModelMatrix(shipModelMatrix, t.shipHeave-p3{t.modelSailPosition.x,0,0});
	shader3D.setUniform("u_Model", shipModelMatrix);

	shader3D.setUniform("u_Color", 0.1, 0.1, 0.1, 1.0f);
	shader3D.setUniform("u_Color", 1, 1, 1, 1.0f);
	shader3D.setUniform("u_Color", 0.9,0.9,0.9, 1.0f);
	lourdesModel.casco.draw();
	lourdesModel.patines.draw();
	shader3D.setUniform("u_Color", 0.1, 0.1, 0.1, 1.0f);
	lourdesModel.orza.draw();
	shader3D.setUniform("u_Color", 0.0f / 255.0f, 63.0f / 255.0f, 200.0f / 255.0f, 1.0f);
	shader3D.setUniform("u_Color", 0.1, 0.1, 0.1, 1.0f);

	lourdesModel.soportes.draw();



	//Rudder
	{
		std::array<float, 16> rudderMatrix = identityMatrix;

		rotate3DModelMatrix(rudderMatrix, t.rudderAngle, { 0,1,0 });
		translate3DModelMatrix(rudderMatrix, t.shipHeave - p3{ t.modelSailPosition.x,0,0 });

		shader3D.setUniform("u_Model", rudderMatrix);
		shader3D.setUniform("u_Color", 0.1, 0.1, 0.1, 1.0f);
		lourdesModel.timon.draw();
	}
	//Sail
	{
		std::array<float, 16> sailMatrix = identityMatrix;

		rotate3DModelMatrix(sailMatrix, t.sailAngle, { 0,1,0 });
		translate3DModelMatrix(sailMatrix, t.sailPosition - p3{ t.modelSailPosition.x,0,0 });

		shader3D.setUniform("u_Model", sailMatrix);

		shader3D.setUniform("u_Color", 137.0f / 255.0f, 18.0f / 255.0f, 18.0f / 255.0f, 1.0f);
		lourdesModel.vela.draw();
	}
	shader3D.setUniform("u_Model", identityMatrix);
}





void TelemetryUI::drawWater()
{
	transparent();
	shader3D.setUniform("u_Model", identityMatrix);
	shader3D.setUniform("u_fragmentMode", shadeColor);
	shader3D.setUniform("u_Color", 40.0f / 255.0f, 189.9f / 255.0f, 255.0f / 255.0f, 0.6);
	water.draw();
	opaque();
}

void TelemetryUI::updateSunLocation()
{
	shader3D.bind();
	shader3D.setUniform("u_lightPos", sun.sunPos);
}

void TelemetryUI::drawSun()
{
	opaque();
	shader3D.bind();
	shader3D.setUniform("u_Model", identityMatrix);
	shader3D.setUniform("u_fragmentMode", flatColor);
	shader3D.setUniform("u_Color", 1.0, 0.60, 0.05, 1);
	sun.draw();
}

void TelemetryUI::drawCenterCross()
{
	transparent();
	shader2D.bind();
	shader2D.setUniform("u_Model", identityMatrix);
	shader2D.setUniform("u_Color", 1, 1, 1, 0.5);
	glLineWidth(1);
	centerCross.draw();
	glLineWidth(1);
	opaque();
}


