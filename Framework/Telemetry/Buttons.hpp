#pragma once

//All the buttons of the program, must have a definition in main to access the InputGLFW functionality


//name identifier for each button of the program
enum class ButtonID
{
	None,
	Close,
	Minimize,
	Drag,
	Ship,
	RouteID, //It already exists a struct called Route
};
using enum ButtonID;

//stores a hitbox, a polygon and a ButtonID
struct Button
{
	ButtonID id = None;
	vector<p2> hitBoxSquare;
	Polygons2D square;

	Button(ButtonID id_, p2 corner1, p2 corner2)
		:id(id_)
	{
		square.addRectangle(corner1, corner2);
		hitBoxSquare = square.positions;
	}

	void draw()
	{
		square.draw();
	}
};



//all Buttons of the program
struct Buttons
{
	float tbh; //Top bar height //needs to be defined before the buttons that use it


	//all buttons
	Button buttonClose, buttonMinimize;
	Lines2D minimizeLines, closeLines;

	Button buttonDrag;
	Text windowName;

	Button buttonShip, buttonCourse;
	Text shipText, courseText;
	Lines2D shipLine, courseLine;

	//each loop it looks if we are over a button
	ButtonID currentHoveredID = None;
	ButtonID currentPressedID = None;


	Buttons()
		: tbh(20)
		, buttonClose(Close, p2{ windowWidth - 20 * (float)sqrt2, windowHeight - tbh }, p2{ windowWidth, windowHeight })
		, buttonMinimize(Minimize, p2{ windowWidth - 40 * (float)sqrt2, windowHeight - tbh }, p2{ windowWidth - 20 * (float)sqrt2, windowHeight })
		, buttonDrag(Drag, p2{ 0, windowHeight - tbh }, p2{ windowWidth, windowHeight }) //can be full width because hitBox check goes after the other buttons
		, buttonShip(Ship, p2{ 0,windowHeight - tbh - 30 }, p2{ 100, windowHeight - tbh })
		, buttonCourse(RouteID, p2{ 100,windowHeight - tbh - 30 }, p2{ 200, windowHeight - tbh })
	{


		windowName.createAtlas(17);
		windowName.addCenteredText({ {windowWidth/2, windowHeight - 17}, "Telemetry Lourdes" });

		shipText.createAtlas(15);
		shipText.addCenteredText({ {50, windowHeight - tbh - 30 + 8}, "Ship" });

		courseText.createAtlas(15);
		courseText.addCenteredText({ {150, windowHeight - tbh - 30 + 8}, "Route" });

		shipLine.addSet({ {0,windowHeight - tbh - 30},{100,windowHeight - tbh - 30} });
		courseLine.addSet({ {100,windowHeight - tbh - 30},{200,windowHeight - tbh - 30} });

		minimizeLines.addSet({ { windowWidth - 35 * (float)sqrt2, windowHeight - 10 }, { windowWidth - 25 * (float)sqrt2, windowHeight - 10 } });

		int cld = 5; //close button lines distance+
		p2 mp = { windowWidth - 10 * (float)sqrt2,windowHeight - 20 / 2 };//middle position of the close button
		closeLines.addSet({ {mp.x - cld,mp.y - cld},{mp.x + cld,mp.y + cld} });
		closeLines.addSet({ {mp.x - cld,mp.y + cld},{mp.x + cld,mp.y - cld} });


	}

	void update()
	{
		currentHoveredID = checkHitBoxes(mPos);
	}

	void draw(Shader& shader2D, Shader& shaderText)
	{

		transparent();
		shader2D.bind();
		shader2D.setUniform("u_Model", identityMatrix);


		shader2D.setUniform("u_Color", 0.35f, 0.35f, 0.35f, 1.0f);
		buttonDrag.draw();
		shaderText.bind();
		windowName.draw();

		shader2D.bind();
		colorButton(buttonClose, shader2D);
		colorButton(buttonMinimize, shader2D);
		shader2D.setUniform("u_Color", 1, 1, 1, 1);
		minimizeLines.draw();
		closeLines.draw();


		colorButton(buttonShip, shader2D);
		colorButton(buttonCourse, shader2D);
		shader2D.setUniform("u_Color", 40.0f / 255.0f, 239.9f / 255.0f, 239.0f / 255.0f, 1);
		if (programState == ship)
			shipLine.draw();
		else if (programState == route)
			courseLine.draw();

		shaderText.bind();
		shipText.draw();
		courseText.draw();
	}


	//to know where I am if a mouse button get's pressed
	ButtonID checkHitBoxes(const p2& m)
	{
		if (isInsideHitBox(buttonClose, m)) return buttonClose.id;
		if (isInsideHitBox(buttonMinimize, m)) return buttonMinimize.id;
		if (isInsideHitBox(buttonDrag, m)) return buttonDrag.id;
		if (isInsideHitBox(buttonShip, m)) return buttonShip.id;
		if (isInsideHitBox(buttonCourse, m)) return buttonCourse.id;

		return None;
	}

	bool isInsideHitBox(const Button& b, const p2& m)
	{
		return (m.x > b.hitBoxSquare[0].x && m.x < b.hitBoxSquare[1].x
			&& m.y > b.hitBoxSquare[0].y && m.y < b.hitBoxSquare[2].y);
	}

	//changes the color if we are hovering, pressing or none to a button
	//In a hardcoced way, maybe it will be more customizable in the future 
	void colorButton(Button& b, Shader& shader2D)
	{
		if (currentPressedID == b.id)
			shader2D.setUniform("u_Color", 0.35f, 0.35f, 0.35f, 1.0f);
		else if (currentHoveredID == b.id)
			shader2D.setUniform("u_Color", 0.219f, 0.219f, 0.219f, 1);
		else
			shader2D.setUniform("u_Color", 0.121f, 0.121f, 0.121f, 1);

		b.draw();

		

	}



};
