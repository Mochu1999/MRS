#pragma once

#include "Graphics.hpp"
#include "Time.hpp"

//Not use to extact the full data vector
//The rendered data gets cropped automatically, only updating the last point each instance
//current x must be some form of value gotten from tm that always increases
struct PlotTime
{
	//actual plotted value
	float* currentY = NULL;
	//tm.currentTime;
	float* currentX;

	//Rendering values, should only be in instancing and model translations and scales
	float gridWidth = 400, gridHeight = 200;
	float offsetX = 0; //offset is kept at 0 unless rendered data.x is bigger than girdWidth. It keeps the first position in 0
	float minDataY = -30, maxDataY = 30; //distances to y=0
	float scaleX = 50; //makes a second span more x distance
	//Once (maxDataY - minDataY) * scaleY > gridHeight, scaleY get's changed so its limits stay inside the given scope
	float scaleY = 40; //makes currentY more sensible to variation

	//all rendering data points
	Lines2D data;
	vector<unsigned int> indices; //big pool of line indices from which a part of it will be assigned to data.indices

	//outer rounded square
	Lines2D frame;

	//axis of the plot, line that shows your current value, dynamic lines that increase with the time
	Lines2DInstanced axis, currentPointCross, dynamicGridLines;
	p2 frameCorner, axisCorner;
	p2 offsetFrameAxis = { 50,50 };

	float stepDynamicGrid = 1; //a vertical dynamic grid each second
	float lastDynamicGridCounter = 0; //keeps track of last vertical grid line
	//values of current X that are getting drawn in steps of stepDynamicGrid
	deque<float> verticalDynamicGridValues = { 0 }; //also used for the text of dynamic grid //0 to write its value

	Text text;
	string title;




	PlotTime() : text("resources/Glyphs/Helvetica/Helvetica.otf", 16)
	{
		setInitialInstancing();
		setIndicesPool();
	}

	void createPlot(float* currentY_, float* currentX_, p2 frameCorner_, string title_);

	void createPlot(float* currentY_, float* currentX_, p2 frameCorner_, string title_
		, float gridWidth_, float gridHeight_
		, float offsetX_, float minDataY_, float maxDataY_
		, float scaleX_, float scaleY_);


	void update();

	//minDataY, maxDataY, scaleY, offsetX
	void updatePlotLimits();

	void updateData();

	void updateCurrentPointCross();

	void updateDynamicGrid();



	void updateText()
	{
		vector<Line> allText;

		//values left to the y axis. 0, maxDataY and minData respectively
		//THE HARDCODED -40 IS A PROBLEM ONCE THE TEXT IS TOO LONG
			//The only solution I can think of would be to know how much space is available and to set a font that fits it. It's overkill now 2026-05
		vector<Line> yAxisText ={ {{axisCorner.x - 40,axisCorner.y - minDataY * scaleY}, 0},
								{{axisCorner.x - 40,axisCorner.y + (maxDataY - minDataY) * scaleY}, round1d(maxDataY)},
								{{axisCorner.x - 40,axisCorner.y }, round1d(minDataY)} };

		//currentX and currentY
		vector<Line> dataText = 
			{{ {axisCorner.x + *currentX * scaleX - offsetX,axisCorner.y - 20},round1d(*currentX)," s"},
			{ {axisCorner.x + *currentX * scaleX - offsetX + 10,axisCorner.y + (data.positions.back().y - minDataY) * scaleY},round1d(*currentY)}};

		vector<Line> verticalDynamicGridText;
		for (size_t i = 0; i < verticalDynamicGridValues.size(); i++)
		{
			verticalDynamicGridText.push_back(
				{ {axisCorner.x - offsetX + verticalDynamicGridValues[i] * scaleX - 10,axisCorner.y - 20},round1d(verticalDynamicGridValues[i]) }
			);
		}

		vector<Line> titleText = {{ {axisCorner.x + 20,axisCorner.y + gridHeight + 10}," ",title} };

		allText.insert(allText.end(), yAxisText.begin(), yAxisText.end());
		allText.insert(allText.end(), dataText.begin(), dataText.end());
		allText.insert(allText.end(), verticalDynamicGridText.begin(), verticalDynamicGridText.end());
		allText.insert(allText.end(), titleText.begin(), titleText.end());
		text.addDynamicText(allText);
	}

	void setInitialInstancing();

	void setIndicesPool();

	void draw(Shader& shader2D, Shader& shader2DInstanced, Shader& shaderText);
};






