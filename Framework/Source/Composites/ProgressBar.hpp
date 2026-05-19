#pragma once

struct ProgressBar
{
	float* percentage = NULL; //linked value from 0 to 1 that will be linked to the PB
	string title;

	//outer line
	p2 outerCorner; //Bottom left corner where the outerRoundedSquare starts
	p2 outerLength = { 500,50 };
	Lines2D outerRoundedSquare;

	//inner polygon
	Polygons2D innerRoundedSquare;

	//Title and percentage in text
	Text text;

	ProgressBar()
		: text("resources/Glyphs/Helvetica/Helvetica.otf", 20)
	{

	}

	void textUpdate()
	{
		std::ostringstream ss; //stream because using directly std::to_string will ignore round1d
		ss << round1d(*percentage * 100) << " %";
		string percentageText = ss.str();

		text.addDynamicText(
			{ { {outerCorner.x + 30,outerCorner.y + outerLength.y + 10} , title},
			{ {outerCorner.x + outerLength.x * *percentage,outerCorner.y - 25} ,percentageText}});
	}

	void createPB(float* percentage_, p2 outerCorner_, string title_)
	{
		percentage = percentage_;
		outerCorner = outerCorner_;
		title = title_;

		//outer line
		outerRoundedSquare.addSet(createRoundedSquare(outerCorner, outerLength.x, outerLength.y, 25));

		//inner polygon
		innerRoundedSquare.addSet(createRoundedSquare(outerCorner, outerLength.x, outerLength.y, 25));
	}
};
