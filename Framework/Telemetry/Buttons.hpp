#pragma once

//All the buttons of the program, must have a definition in main to access the InputGLFW functionality


//name identifier for each button of the program
enum class ButtonID
{
	None,
	Close,
	Minimize,
	WindowDrag,
	Ship,
	RouteID, //It already exists a struct called Route
	RudderText,
	SailText,
	RudderBar,
	SailBar,
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

struct TextField : Button
{
	Text text;
	float& value;
	p2 bottomCenter;

	//0 usual state //1 you are writing a new angle //2 you have written a new angle and have pressed enter
	int state = 0;
	float newValue = 0; //the one your write

	bool decimal = false;
	float decimalFactor = 0.1f;
	bool negative = false;

	TextField(ButtonID id_, p2 corner1, p2 corner2, float& value_)
		: Button(id_, corner1, corner2), value(value_)
	{
		bottomCenter = { corner1.x + (corner2.x - corner1.x) / 2,corner1.y + 5 };
		text.createAtlas(36, "resources/Glyphs/Helvetica/Helvetica.otf");


	}
	void drawTextField(Shader& shaderText)
	{
		if (state == 2)
		{
			value = newValue;
			newValue = 0;
			state = 0;
		}

		shaderText.bind();
		text.clear();
		if (state == 0)
		{
			TextEntry textEntry(bottomCenter, round1d(value));
			text.addCenteredText(textEntry);

		}
		else if (state == 1)
		{
			TextEntry textEntry(bottomCenter, newValue);
			text.addCenteredText(textEntry);

		}

		text.draw();

	}
};

struct DragBar
{
	ButtonID id = None;

	p2 barCenter; //fixed
	p2 squareCenter; //moves
	float squareWidth = 18;

	float distance = 100; //bar goes distance left/right

	float& value;

	Polygons2D square;
	Polygons2D bar;

	DragBar(ButtonID id_, p2 center_, float& value_)
		: id(id_), barCenter(center_), squareCenter(center_), value(value_)
	{
		square.addRectangle(
			{ squareCenter.x - squareWidth, squareCenter.y - squareWidth },
			{ squareCenter.x + squareWidth, squareCenter.y + squareWidth });

		bar.addRectangle(
			{ barCenter.x - distance, barCenter.y - 3 },
			{ barCenter.x + distance, barCenter.y + 3 });
	}

	void drag(float mouseX)
	{
		float newX = mouseX;

		if (newX < barCenter.x - distance)
			newX = barCenter.x - distance;
		else if (newX > barCenter.x + distance)
			newX = barCenter.x + distance;

		float movement = newX - squareCenter.x;
		squareCenter.x = newX;

		for (p2& p : square.positions)
			p.x += movement;

		// left = -90º, center = 0º, right = +90º
		value = 90.0f * (squareCenter.x - barCenter.x) / distance;
	}

	void draw(Shader& shader2D, ButtonID currentPressedID, ButtonID currentHoveredID)
	{
		shader2D.setUniform("u_Color", almostWhite, 1);
		bar.draw();

		if (currentPressedID == id)
			shader2D.setUniform("u_Color", 20.0f / 255.0f, 120.0f / 255.0f, 180.0f / 255.0f, 1);
		else if (currentHoveredID == id)
			shader2D.setUniform("u_Color", 80.0f / 255.0f, 210.0f / 255.0f, 255.0f / 255.0f, 1);
		else
			shader2D.setUniform("u_Color", 40.0f / 255.0f, 190.0f / 255.0f, 255.0f / 255.0f, 1);

		matrix4x4 modelMatrix = identityMatrix;
		translate2DModelMatrix(modelMatrix, p2{ squareCenter.x - barCenter.x, 0 });

		shader2D.setUniform("u_Model", modelMatrix);
		square.draw();
		shader2D.setUniform("u_Model", identityMatrix);

	}
};


//STRUCTS CON TEXTURAS Y E ICONOS PARA MINIMIZAR Y CERRAR

//Los botones deberían de vivir en UI, junto con la struct de su zona
//Las hitboxes no deberían de comprobarse siempre, solo mientras esté visible sus botones (no mostrar si te vas a route)




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

	TextField fieldRudder, fieldSail;

	DragBar barRudder,barSail;

	//each loop it looks if we are over a button
	ButtonID currentHoveredID = None;
	ButtonID currentPressedID = None;


	Buttons(float& rudderAngle, float& sailAngle)
		: tbh(20)
		, buttonClose(Close, p2{ windowWidth - 20 * (float)sqrt2, windowHeight - tbh }, p2{ windowWidth, windowHeight })
		, buttonMinimize(Minimize, p2{ windowWidth - 40 * (float)sqrt2, windowHeight - tbh }, p2{ windowWidth - 20 * (float)sqrt2, windowHeight })
		, buttonDrag(WindowDrag, p2{ 0, windowHeight - tbh }, p2{ windowWidth, windowHeight }) //can be full width because hitBox check goes after the other buttons
		, buttonShip(Ship, p2{ 0,windowHeight - tbh - 30 }, p2{ 100, windowHeight - tbh })
		, buttonCourse(RouteID, p2{ 100,windowHeight - tbh - 30 }, p2{ 200, windowHeight - tbh })
		, fieldRudder(RudderText, p2{ 50,500 }, p2{ 140,536 }, rudderAngle)
		, fieldSail(SailText, p2{ 50,600 }, p2{ 140,636 }, sailAngle)
		, barRudder(RudderBar, p2{ 300,518 }, rudderAngle)
		, barSail(SailBar, p2{ 300,618 }, sailAngle)
	{


		windowName.createAtlas(17);
		windowName.addCenteredText({ {windowWidth / 2, windowHeight - 17}, "Telemetry Lourdes" });

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

		//fieldRudder.drawTextField(shader2D, shaderText, currentPressedID, currentHoveredID);


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

		//rudder
		colorButton(fieldRudder, shader2D);
		fieldRudder.drawTextField(shaderText);
		shader2D.bind();

		barRudder.draw(shader2D, currentPressedID, currentHoveredID);
		barSail.draw(shader2D, currentPressedID, currentHoveredID);

		//sail
		colorButton(fieldSail, shader2D);
		fieldSail.drawTextField(shaderText);
		shader2D.bind();

		//Ship and course
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
		if (isInsideHitBox(fieldRudder, m)) return fieldRudder.id;
		if (isInsideHitBox(fieldSail, m)) return fieldSail.id;
		if (isInsideHitBox(barRudder, m)) return barRudder.id;
		if (isInsideHitBox(barSail, m)) return barSail.id;

		return None;
	}

	bool isInsideHitBox(const Button& b, const p2& m)
	{
		return (m.x > b.hitBoxSquare[0].x && m.x < b.hitBoxSquare[1].x
			&& m.y > b.hitBoxSquare[0].y && m.y < b.hitBoxSquare[2].y);
	}

	bool isInsideHitBox(const DragBar& b, const p2& m)
	{
		return (
			m.x > b.squareCenter.x - 18 &&
			m.x < b.squareCenter.x + 18 &&
			m.y > b.squareCenter.y - 18 &&
			m.y < b.squareCenter.y + 18
			);
	}

	//changes the color if we are hovering, pressing or none to a button
	//In a hardcoced way, maybe it will be more customizable in the future 
	void colorButton(Button& b, Shader& shader2D)
	{
		if (currentPressedID == b.id) //pressed
			shader2D.setUniform("u_Color", 0.35f, 0.35f, 0.35f, 1.0f);
		else if (currentHoveredID == b.id) //hovering
			shader2D.setUniform("u_Color", 0.219f, 0.219f, 0.219f, 1);
		else //normal state
			shader2D.setUniform("u_Color", grey, 1);

		b.draw();
	}



};
