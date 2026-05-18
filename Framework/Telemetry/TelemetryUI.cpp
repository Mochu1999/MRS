#include "TelemetryUI.hpp"
//#include "Graphics.hpp"

void TelemetryUI::drawLourdes()
{


	shader3D.bind();
	opaque();
	shader3D.setUniform("u_fragmentMode", shadeColor);


	//static elements are translated with shipHeave
	camera.translate3DModelMatrix(shipModelMatrix, t.shipHeave);
	shader3D.setUniform("u_Model", shipModelMatrix);

	shader3D.setUniform("u_Color", 0.1, 0.1, 0.1, 1.0f);
	lourdesModel.casco.draw();
	lourdesModel.patines.draw();
	shader3D.setUniform("u_Color", 137.0f / 255.0f, 18.0f / 255.0f, 18.0f / 255.0f, 1.0f);
	lourdesModel.orza.draw();
	shader3D.setUniform("u_Color", 0.0f / 255.0f, 63.0f / 255.0f, 200.0f / 255.0f, 1.0f);
	lourdesModel.soportes.draw();



	//Rudder
	{
		std::array<float, 16> rudderMatrix = identityMatrix;

		camera.rotate3DModelMatrix(rudderMatrix, t.rudderAngle, { 0,1,0 });
		camera.translate3DModelMatrix(rudderMatrix, t.shipHeave);

		shader3D.setUniform("u_Model", rudderMatrix);
		shader3D.setUniform("u_Color", 113.0f / 255.0f, 10.0f / 255.0f, 87.0f / 255.0f, 1.0f);
		lourdesModel.timon.draw();
	}
	//Sail
	{
		std::array<float, 16> sailMatrix = identityMatrix;

		camera.rotate3DModelMatrix(sailMatrix, t.sailAngle, { 0,1,0 });
		camera.translate3DModelMatrix(sailMatrix, t.sailPosition);

		shader3D.setUniform("u_Model", sailMatrix);

		shader3D.setUniform("u_Color", 137.0f / 255.0f, 18.0f / 255.0f, 18.0f / 255.0f, 1.0f);
		lourdesModel.vela.draw();
	}
	shader3D.setUniform("u_Model", identityMatrix);
}





void TelemetryUI::drawAxis()
{
	shader3D.bind();
	shader3D.setUniform("u_Model", identityMatrix);

	shader3D.setUniform("u_fragmentMode", 1);


	glLineWidth(1);
	shader3D.setUniform("u_Color", 1.0, 0.0, 0.0, 1.0);
	axis.xLine.draw();
	shader3D.setUniform("u_Color", 0.0, 1.0, 0.0, 1.0);
	axis.yLine.draw();
	shader3D.setUniform("u_Color", 0.0, 0.0, 1.0, 1.0);
	axis.zLine.draw();
	shader3D.setUniform("u_Color", 1, 1, 1, 1.0);

	glLineWidth(1);

	shader3D.setUniform("u_Color", 1, 1, 1, 1.0);

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

void TelemetryUI::drawOverlay()
{
	shader2D.bind();
	transparent();
	shader2D.setUniform("u_Model", identityMatrix);
	shader2D.setUniform("u_Color", 0.035f, 0.065f, 0.085f, 1.0f);
	overlay.background.draw();

	shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
	glLineWidth(5);
	shader2D.setUniform("u_Model", identityMatrix);
	overlay.backgroundLines.draw();
	glLineWidth(1);
}