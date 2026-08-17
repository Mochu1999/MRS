#pragma once

#include "Graphics.hpp"


struct Lourdes3DModel
{

	Polyhedra casco, patines, vela, orza, soportes, timon;

	Telemetry& t;
	Lines3D headingLine;


	Lourdes3DModel(Telemetry& t_)
		:t(t_)
	{
		/*stlToSimplePolyhedra("casco.stl", "casco.bin");
		stlToSimplePolyhedra("patines.stl", "patines.bin");
		stlToSimplePolyhedra("vela.stl", "vela.bin");
		stlToSimplePolyhedra("timon.stl", "timon.bin");
		stlToSimplePolyhedra("orza.stl", "orza.bin");
		stlToSimplePolyhedra("soportes.stl", "soportes.bin");*/

		casco.addPolyhedra("casco.bin");
		patines.addPolyhedra("patines.bin");
		vela.addPolyhedra("vela.bin");
		orza.addPolyhedra("orza.bin");
		soportes.addPolyhedra("soportes.bin");
		timon.addPolyhedra("timon.bin");

		headingLine.addSet({ { 0.8,0.15,0 },{2,0.15,0} });
	}

	void draw(Shader& shader3D)
	{
		std::array<float, 16> shipModelMatrix = identityMatrix;


		shader3D.bind();
		opaque();
		shader3D.setUniform("u_fragmentMode", shadeColor);


		rotate3DModelMatrix(shipModelMatrix, t.headingAngle, { 0,1,0 });
		translate3DModelMatrix(shipModelMatrix, t.shipHeave);
		shader3D.setUniform("u_Model", shipModelMatrix);

		shader3D.setUniform("u_Color", 1, 0, 0, 1.0f);
		headingLine.draw();
		shader3D.setUniform("u_Color", 1, 1, 1, 1.0f);
		shader3D.setUniform("u_Color", 0.9, 0.9, 0.9, 1.0f);
		casco.draw();
		patines.draw();
		shader3D.setUniform("u_Color", 0.1, 0.1, 0.1, 1.0f);
		orza.draw();
		shader3D.setUniform("u_Color", 0.0f / 255.0f, 63.0f / 255.0f, 200.0f / 255.0f, 1.0f);
		shader3D.setUniform("u_Color", 0.1, 0.1, 0.1, 1.0f);

		soportes.draw();



		//Rudder
		{
			std::array<float, 16> rudderMatrix = identityMatrix;

			rotate3DModelMatrix(rudderMatrix, t.headingAngle, { 0,1,0 });
			rotate3DModelMatrix(rudderMatrix, t.rudderAngle, { 0,1,0 });
			translate3DModelMatrix(rudderMatrix, t.shipHeave);

			shader3D.setUniform("u_Model", rudderMatrix);
			shader3D.setUniform("u_Color", 0.1, 0.1, 0.1, 1.0f);
			timon.draw();
		}
		//Sail
		{
			//Operations are: Theave-Rsail-TsailPosition-Rheading
			// But to end with that transformation order must be reversed: p3 = Rheading * Tsail * Rsail * pos 
			// (shipHeave can be applied wherever, doesn't interact with the others)

			std::array<float, 16> sailMatrix = identityMatrix;

			rotate3DModelMatrix(sailMatrix, t.headingAngle, { 0,1,0 });
			translate3DModelMatrix(sailMatrix, t.sailPosition);
			rotate3DModelMatrix(sailMatrix, t.sailAngle, { 0,1,0 });
			translate3DModelMatrix(sailMatrix, t.shipHeave);


			shader3D.setUniform("u_Model", sailMatrix);

			shader3D.setUniform("u_Color", 137.0f / 255.0f, 18.0f / 255.0f, 18.0f / 255.0f, 1.0f);
			vela.draw();
		}
		shader3D.setUniform("u_Model", identityMatrix);
	}
};