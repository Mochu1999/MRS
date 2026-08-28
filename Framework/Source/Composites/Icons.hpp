#pragma once
#include "Telemetry.hpp"

struct Ship2DIcon
{
	Polygons2D casco2D, patines2D, sideHull2, vela2D, soportes2D, timon2D;

	Ship2DIcon()
	{
		Polyhedra interm;

		interm.addSTL("casco2D.stl");
		polyhedraTo2D(interm, casco2D);

		interm.addSTL("patines2D.stl");
		polyhedraTo2D(interm, patines2D);

		interm.addSTL("vela2D.stl");
		polyhedraTo2D(interm, vela2D);

		interm.addSTL("soportes2D.stl");
		polyhedraTo2D(interm, soportes2D);

		interm.addSTL("timon2D.stl");
		polyhedraTo2D(interm, timon2D);
	}

	void draw(World& world, Telemetry& t, Shader& shader2D)
	{
		//Only for visual projection because the one from 
		p2 finish = lonLatToMercator(t.finishPoint);
		p2 current = lonLatToMercator(t.position);

		p2 delta = finish - current;
		float headingAngleProjection = degrees(atan2(delta.y, delta.x));



		shader2D.bind();

		//Ship model matrix, the definitive for the static parts of the ship
		//Friendly reminder that the transformations that are written first are the last to be applied
		//For the ship the order should be:
		// S ship - R heading - T shipPosition - S world - T world
		//That is, that we first apply the ship transformations and then we put it in the map
		matrix4x4 ship2DMatrix = identityMatrix;
		translate2DModelMatrix(ship2DMatrix, world.translationModel);
		scale2DModelMatrix(ship2DMatrix, world.scaleModel);
		translate2DModelMatrix(ship2DMatrix, lonLatToMercator(t.position));
		rotate2DModelMatrix(ship2DMatrix, headingAngleProjection);
		scale2DModelMatrix(ship2DMatrix, 100000); //Arbitrary so the icon gets big

		//We build and draw the rudder matrix first as its drawing goes under
		matrix4x4 rudderMatrix = ship2DMatrix;

		rotate2DModelMatrix(rudderMatrix, t.rudderAngle);
		scale2DModelMatrix(rudderMatrix, 4); //Otherwise the rudder is not seen from above
		shader2D.setUniform("u_Model", rudderMatrix);
		shader2D.setUniform("u_Color", 1, 1, 1, 1.0f);

		timon2D.draw();


		shader2D.setUniform("u_Model", ship2DMatrix);

		shader2D.setUniform("u_Color", 1, 1, 1, 1.0f);
		soportes2D.draw();

		shader2D.setUniform("u_Color", 1,1,0, 1.0f);
		casco2D.draw();
		patines2D.draw();
		sideHull2.draw();

		


		//Sail model matrix
		matrix4x4 sail2DMatrix = ship2DMatrix;

		translate2DModelMatrix(ship2DMatrix, { t.sailPositionVisual.x,t.sailPositionVisual.z });
		rotate2DModelMatrix(ship2DMatrix, t.sailAngle);
		shader2D.setUniform("u_Model", ship2DMatrix);
		shader2D.setUniform("u_Color", 1, 1, 1, 1.0f);
		vela2D.draw();


	}
};






struct ArrowsMeteo
{
	Lines2DInstanced lines;
	
	//Normalized arrow
	vector<p2> positions = { {0,0},{0,-0.019},{0.88,-0.019},{0.714,-0.145},{0.742,-0.175},{1,0}
		, { 0.742,0.175 },{0.714,0.145},{0.88,0.019},{0,0.019},{0,0} };

	vector<unsigned int> indices = { 0,1,2 ,2,3,4 ,2,4,5, 0,2,5 ,5,6,7 ,5,7,8 ,5,8,0, 8,9,0 };

	ArrowsMeteo()
	{

		lines.addInitialSet(positions);
		lines.addInstances({ {  { 50,50 },0,{100,100} } });

	}

	void update(const vector<InstanceAttributes>& currentInstancing)
	{
		lines.clear();
		lines.addInitialSet(positions);
		lines.addInstances(currentInstancing);
	}

	
};