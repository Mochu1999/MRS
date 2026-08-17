#pragma once
#include "Common.hpp"
#include "Graphics.hpp"
#include "Lines2D.hpp"
#include "AuxLonLats.hpp"



//Pan, zoom in, zoom out
//Correct Safe zones //Safe zones stored in binary

//Stores landmass coordinates and visualizes all world related variables

//Coordinates of the world are stored in meters
//Assumes the map is there and that you are able to read it
struct World
{
	vector<vector<p2>> allLonLats, safeZones; //each inner vector is a landmass made of vertices in ccw

	Lines2D mercator, safeZonesMercatos; //proyected map, only for visualization
	Lines2D frame; //outer rectangle of mercator

	Polygons2D background;

	//Visualization variables
	p2 bottomLeftCorner; 
	float totalXmeters, totalYmeters; //horizontal and vertical distances
	//float totalXpixels = 1920;// 6000; //Setting arbitrarily how many horizontal pixels the map is (can be scaled up or down later)
	float totalXpixels = 6000; //Setting arbitrarily how many horizontal pixels the map is (can be scaled up or down later)

	//scaling and translating the map with u_Model for visualization
	float scaleModel;
	p2 mapCorner, translationModel;

	matrix4x4 mapModel2DMatrix;

	

	World()
	{
		processBinaryMap("mediterranean.txt");

		createSafeZone(0.05);
		
		initVisualizationVariables();

		updateCamera();
	}

	//Fills allLonLats, mercator, frame, background and the initial va
	void processBinaryMap(const std::string& localPath);

	void initVisualizationVariables()
	{
		bottomLeftCorner = frame.positions[0];
		totalXmeters = frame.positions[1].x - frame.positions[0].x;
		totalYmeters = frame.positions[2].y - frame.positions[1].y;

		//Multiplying the map positions by scaleModel in the model matrix will make all coordinates fall between 0 and totalXpixels (6000 pixels)
		scaleModel = totalXpixels / totalXmeters;

		//Making the bottom left corner of the screen be the bottom left corner of the map
		translationModel = -bottomLeftCorner * scaleModel;
		
		//bottomLeftCorner is at the bottom left of the screen, now we move it arbitrarily some amount so it's centered
		translationModel += centerWindow - (totalXpixels * 0.5f);
	}

	//We have maps of over 10^6 meters, and we want totalXpixels (arbitrarily) to be of a certain size after scrolling
	void updateCamera()
	{
		// World is at center
		p2 worldCenter = (centerWindow - translationModel) / scaleModel;

		//Scrolling the mouse's wheel changes totalXpixels
		scaleModel = totalXpixels / totalXmeters;

		// When
		translationModel = centerWindow - worldCenter * scaleModel;
	}

	void draw(Shader& shader2D)
	{
		shader2D.bind();
		transparent();

		mapModel2DMatrix = identityMatrix;
		translate2DModelMatrix(mapModel2DMatrix, translationModel);
		scale2DModelMatrix(mapModel2DMatrix, scaleModel);

		shader2D.setUniform("u_Model", mapModel2DMatrix);
		shader2D.setUniform("u_Color", 0.035f, 0.065f, 0.085f, 1.0f);
		background.draw();

		shader2D.setUniform("u_Color", 40 / 255.0f, 40 / 255.0f, 40 / 255.0f, 1.0f);

		shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
		mercator.draw();

		shader2D.setUniform("u_Color", 1, 0, 0, 1);
		safeZonesMercatos.draw();


		glLineWidth(3);
		frame.draw();
		glLineWidth(1);
	}

	//Calculates safeZones coordinates with the given distances and creates its mercactor projection
	// Esto no es preciso, debería de hacerse Per-vertex local tangent plane
	// Bisector may fail, not limit tested
	//offsetted polygon around input, each point of it falls in the bisector angle
	void createSafeZone(float distance);

	//Auxiliar of createSafeZone
	void removeInnerLoops(vector<p2>& polygon);


};


