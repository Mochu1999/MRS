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

	void draw(Shader& shader2D, Shader& shaderText)
	{
		shader2D.bind();
		shader2D.setUniform("u_Model", identityMatrix);


		//outer line
		shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
		glLineWidth(3);
		outerRoundedSquare.draw();
		glLineWidth(1);

		//inner polygon
		shader2D.setUniform("u_Color", 0.7, 0.7, 0.7, 1);
		glEnable(GL_SCISSOR_TEST); //method to cut an object just for the rendering
		glScissor(outerCorner.x - 10, 0, 10 + outerLength.x * (*percentage), windowHeight);
		innerRoundedSquare.draw();
		glDisable(GL_SCISSOR_TEST);

		textUpdate();

		shaderText.bind();
		text.draw();
	}
};
