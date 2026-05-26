#pragma once

//All the buttons of the program, must have a definition in main to access the InputGLFW functionality


//name identifier for each button of the program
enum class ButtonID
{
	None,
	Close,
	Minimize,
	Drag
};
using enum ButtonID;

//stores a hitbox, a polygon and a ButtonID
//ICONS
struct Button
{
	ButtonID id = None;
	vector<p2> hitBoxSquare;
	Polygons2D square;

	Button(ButtonID id_, p2 corner1, p2 corner2)
		:id(id_)
	{
		square.createRectangle(corner1, corner2);
		hitBoxSquare = square.positions;
	}

	void draw()
	{
		square.draw();
	}
};



//all Buttons in the program
struct Buttons
{
	//all buttons
	Button buttonClose;
	Button buttonMinimize;

	Button buttonDrag;
	Text windowName;

	//each loop it looks if we are over a button
	ButtonID currentHoveredID = None;
	ButtonID currentPressedID = None;


	Buttons()
		: buttonClose(Close, p2{ windowWidth - 20 * (float)sqrt2, windowHeight - 20 }, p2{ windowWidth, windowHeight })
		, buttonMinimize(Minimize, p2{ windowWidth - 40 * (float)sqrt2, windowHeight - 20 }, p2{ windowWidth - 20 * (float)sqrt2, windowHeight })
		, buttonDrag(Drag, p2{ 0, windowHeight - 20 }, p2{ windowWidth, windowHeight }) //can be full width because hitBox check goes after the other buttons
	{
		windowName.createAtlas(14);
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
		windowName.addDynamicText({ { {10, windowHeight - 17}, "Telemetry Lourdes" } });
		windowName.draw();
		shader2D.bind();

		colorButton(buttonClose, shader2D);
		colorButton(buttonMinimize, shader2D);

	}


	//to know where I am if mouse button get's pressed
	ButtonID checkHitBoxes(const p2& m)
	{
		if (isInsideHitBox(buttonClose, m)) return buttonClose.id;
		else if (isInsideHitBox(buttonMinimize, m)) return buttonMinimize.id;
		else if (isInsideHitBox(buttonDrag, m)) return buttonDrag.id; //if drag is after close and minimize, they won't interact

		else return None;
	}

	bool isInsideHitBox(const Button& b, const p2& m)
	{
		return (m.x > b.hitBoxSquare[0].x && m.x < b.hitBoxSquare[1].x
			&& m.y > b.hitBoxSquare[0].y && m.y < b.hitBoxSquare[2].y);
	}

	//changes the color if we are hovering, pressing or none to a button
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
