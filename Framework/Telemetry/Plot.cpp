#include "Plot.hpp"
#include "Graphics.hpp"
#include "Time.hpp"


void PlotTime::createPlot(float* currentY_, float* currentX_, p2 frameCorner_, string title_)
{
	currentY = currentY_;
	currentX = currentX_;
	frameCorner = frameCorner_;
	title = title_;

	axisCorner = frameCorner + offsetFrameAxis;

	//setting frame
	frame.addSet(createRoundedSquare({ frameCorner.x ,frameCorner.y }
		, gridWidth + 2 * offsetFrameAxis.x, gridHeight + 2 * offsetFrameAxis.y
		, (offsetFrameAxis.x + offsetFrameAxis.y) / 4)); //third variable is just for aesthetics


	//axis will stay static, so it's set here
	axis.addInstances({
		//horizontal
		{  { gridWidth, 1 },  0, axisCorner},
		//vertical
		{  { gridHeight, 1 },  radians(90), axisCorner },
		});

	update();
}

void PlotTime::createPlot(float* currentY_, float* currentX_, p2 frameCorner_, string title_
	, float gridWidth_, float gridHeight_
	, float offsetX_, float minDataY_, float maxDataY_
	, float scaleX_, float scaleY_)
{
	currentY = currentY_;
	currentX = currentX_;
	frameCorner = frameCorner_;
	title = title_;
	gridWidth = gridWidth_;
	gridHeight = gridHeight_;
	offsetX = offsetX_;
	minDataY = minDataY_;
	maxDataY = maxDataY_;
	scaleX = scaleX_;
	scaleY = scaleY_;

	axisCorner = frameCorner + offsetFrameAxis;

	//setting frame
	frame.addSet(createRoundedSquare({ frameCorner.x ,frameCorner.y }
		, gridWidth + 2 * offsetFrameAxis.x, gridHeight + 2 * offsetFrameAxis.y
		, (offsetFrameAxis.x + offsetFrameAxis.y) / 4)); //third variable is just for aesthetics


	//axis will stay static, so it's set here
	axis.addInstances({
		//horizontal
		{  { gridWidth, 1 },  0, axisCorner},
		//vertical
		{  { gridHeight, 1 },  radians(90), axisCorner },
		});

	update();
}

void PlotTime::update()
{
	if (currentY != NULL)
	{
		updatePlotLimits();

		updateData();

		updateCurrentPointCross();

		updateDynamicGrid();

		updateText();
	}
}

void PlotTime::updatePlotLimits()
{
	//distances to y=0, in rendering they will always be multiplied by scaleY
	if (*currentY < minDataY) minDataY = *currentY;
	if (*currentY > maxDataY) maxDataY = *currentY;

	//Data surpases gridHeight and we need to scale it down
	if ((maxDataY - minDataY) * scaleY > gridHeight) scaleY = gridHeight / (maxDataY - minDataY);

	//rendering X surpases gridWidth, and we need to displace the whole data to the left
	if (*currentX * scaleX > gridWidth) offsetX = *currentX * scaleX - gridWidth;
}

void PlotTime::updateData()
{
	data.positions.push_back({ *currentX,*currentY });

	//difference of the rendering distance (that between the first and last data point) must not be higher than gridWidth
	while (scaleX * (data.positions.back().x - data.positions.front().x) >= gridWidth)
	{
		data.positions.erase(data.positions.begin());
	}

	//assigning data.indices from our pool of indices
	if (data.indices.size() != 2 * data.positions.size() - 1)
		data.indices.assign(indices.begin(), indices.begin() + 2 * data.positions.size() - 1);

	data.isBufferUpdated = true;
}

void PlotTime::updateCurrentPointCross()
{
	//scalings to have the length of the last point
	currentPointCross.addInstances({
		//horizontal line
		{ { *currentX * scaleX - offsetX,1 }, 0, {axisCorner.x,axisCorner.y + (data.positions.back().y - minDataY) * scaleY}},
		//vertical line
		{  { (data.positions.back().y - minDataY) * scaleY,1 },radians(90) , { axisCorner.x - offsetX + *currentX * scaleX, axisCorner.y}}
		});
}

void PlotTime::updateDynamicGrid()
{
	vector<InstanceAttributes> auxDynamicGrid; //loading attributes here

	if (*currentX - lastDynamicGridCounter > 1)
	{
		verticalDynamicGridValues.push_back(*currentX);
		lastDynamicGridCounter += 1;
	}

	//horizontal line at y=0
	//translation is axisCorner plus where the rendered 0 is
	auxDynamicGrid.push_back({ { gridWidth, 1 }, 0, {axisCorner.x, axisCorner.y - minDataY * scaleY} });

	//horizontal line at maxDataY
	auxDynamicGrid.push_back({ { gridWidth, 1 },  0, {axisCorner.x,axisCorner.y + (maxDataY - minDataY) * scaleY} });

	//vertical line at gridWidth //this one is actually static, but whatever
	auxDynamicGrid.push_back({ { gridHeight, 1 },  radians(90), {axisCorner.x + gridWidth,axisCorner.y} });

	for (size_t i = 0; i < verticalDynamicGridValues.size(); i++)
	{
		auxDynamicGrid.push_back(
			{ { gridHeight,1 }, radians(90) , { axisCorner.x - offsetX + verticalDynamicGridValues[i] * scaleX,axisCorner.y} });
	}
	dynamicGridLines.addInstances(auxDynamicGrid);

	//poping the front of the verticalDynamicGrid lines if there's more rendering distance between currentX and the front value
	// than gridWidth (and a hardcoded +10 so it dissapears a little before touching axis)
	if (verticalDynamicGridValues.size())
	{
		if ((*currentX - verticalDynamicGridValues.front()) * scaleX + 10 >= gridWidth)
			verticalDynamicGridValues.pop_front();
	}
}



void PlotTime::setInitialInstancing()
{
	vector<p2> initialLine = { {0,0},{1,0} };
	axis.addInitialSet(initialLine);
	currentPointCross.addInitialSet(initialLine);
	dynamicGridLines.addInitialSet(initialLine);
}

void PlotTime::setIndicesPool()
{
	//hardcoding a big indices pool, study edge cases in the future
	size_t maxPositionsSize = 2000;
	data.indices.reserve(maxPositionsSize);
	for (size_t i = 0; i < maxPositionsSize; i++)
	{
		indices.emplace_back(i);
		indices.emplace_back(i + 1);
	}
}

void PlotTime::draw(Shader& shader2D, Shader& shader2DInstanced, Shader& shaderText)
{
	update();

	//Frame
	transparent();
	shader2D.bind();
	shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
	glLineWidth(3);
	shader2DInstanced.setUniform("u_Model", identityMatrix);
	frame.draw();
	glLineWidth(1);

	//Data
	shader2D.bind();
	matrix4x4 modelDataMatrix =
		create2DModelMatrix({ scaleX ,scaleY }, 0, axisCorner - p2{ offsetX,minDataY * scaleY });
	shader2D.setUniform("u_Model", modelDataMatrix);
	shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
	data.draw();
	shader2D.setUniform("u_Model", identityMatrix);

	//Axis
	shader2DInstanced.bind();
	shader2DInstanced.setUniform("u_Model", identityMatrix); //OBJETIVO BORRAR
	shader2D.setUniform("u_Color", 1, 1, 1, 1);
	axis.draw();
	//currentPointCross
	shader2DInstanced.setUniform("u_Color", 1, 0, 0, 1);
	currentPointCross.draw();
	//grid
	shader2DInstanced.setUniform("u_Color", 0.5, 0.5, 0.5, 0.5);
	dynamicGridLines.draw();

	//Text
	shaderText.bind();
	text.draw();

}