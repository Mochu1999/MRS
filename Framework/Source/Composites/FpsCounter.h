#pragma once

//Calculates and draws fps
struct FpsCounter
{
	TimeStruct& tm;

	Text fpsText;

	float fps = 0.0f; //stores the FPS and updates it only after updateFpsIntervalTime
	float updateFpsIntervalTime = 0.5f;

	FpsCounter(TimeStruct& tm_) : tm(tm_)
	{
		fpsText.createAtlas(20);
	}

	void update()
	{
		//Calculates fps  after updateFpsIntervalTime and resets itself
		if (tm.fpsTimeAccumulator >= updateFpsIntervalTime)
		{
			fps = tm.frameCount / tm.fpsTimeAccumulator;
			tm.frameCount = 0;
			tm.fpsTimeAccumulator -= 0.5f;
		}
	}

	void draw(Shader& shaderText)
	{
		update();

		shaderText.bind();
		transparent();
		TextEntry fpsTextEntry({ 50,windowHeight-100}, round2d(fps), " fps");
		fpsText.addDynamicText({ fpsTextEntry });
		fpsText.draw();
	}
};