#pragma once

struct WaterMRS
{
	float waterRadius = 2;

	//IMPLEMENTAR ADDCIRCLES EN POLYGONS3D
	Polygons2D circle;
	Polyhedra water;

	Polygons3D compassLines;

	//Positions of the commpass lines
	//We will rotate the positions with quaternions
	float smallLineLength = 0.4;
	float smallLineWidth = 0.004f; //gets doubled
	vector<p3> positionsSmall = { {waterRadius,0,smallLineWidth},{waterRadius + smallLineLength,0,smallLineWidth},{waterRadius + smallLineLength,0,-smallLineWidth},{waterRadius,0,-smallLineWidth},{waterRadius,0,smallLineWidth} };

	float smLineLength = smallLineLength * 1.2;
	float smLineWidth = smallLineWidth * 2; //gets doubled
	vector<p3> positionsSM = { {waterRadius,0,smLineWidth},{waterRadius + smLineLength,0,smLineWidth},{waterRadius + smLineLength,0,-smLineWidth},{waterRadius,0,-smLineWidth},{waterRadius,0,smLineWidth} };

	float mediumLineLength = smallLineLength * 1.4;
	float mediumLineWidth = smLineWidth; //gets doubled
	vector<p3> positionsMedium = { {waterRadius,0,mediumLineWidth},{waterRadius + mediumLineLength,0,mediumLineWidth},{waterRadius + mediumLineLength,0,-mediumLineWidth},{waterRadius,0,-mediumLineWidth},{waterRadius,0,mediumLineWidth} };

	float bigLineLength = smallLineLength * 2.3;
	float bigLineWidth = smallLineWidth * 4; //gets doubled
	vector<p3> positionsBig = { {waterRadius,0,bigLineWidth},{waterRadius + bigLineLength,0,bigLineWidth},{waterRadius + bigLineLength,0,-bigLineWidth},{waterRadius,0,-bigLineWidth},{waterRadius,0,bigLineWidth} };


	//Text
	Text3D n, ne, e, se, s, sw, w, nw;

	vector<Text3D> angles;
	int anglesStep = 10;//º
	int anglesCount = 360 / anglesStep;

	WaterMRS()
	{
		{
			circle.addCircle(waterRadius, { 0,0 });
			vector<p3> positions = p2ToP3Y(circle.positions);
			vector<p3> normals;

			for (size_t i = 0; i < positions.size(); i++)
			{
				normals.push_back({ 0,1,0 });
			}

			water.addPolyhedra(positions, normals, circle.indices);
		}

		createLinesPolygons();

		//smallLineLength = 1;
		n.createAtlas(smallLineLength);
		n.addCenteredText(Text3DEntry({ waterRadius + bigLineLength,0,0 }, { 0,0,1 }, { 1,0,0 }, "N"));
		ne.createAtlas(smallLineLength);
		ne.addCenteredText(Text3DEntry({ waterRadius + bigLineLength,0,0 }, { 0,0,1 }, { 1,0,0 }, "NE"));
		e.createAtlas(smallLineLength);
		e.addCenteredText(Text3DEntry({ waterRadius + bigLineLength,0,0 }, { 0,0,1 }, { 1,0,0 }, "E"));
		se.createAtlas(smallLineLength);
		se.addCenteredText(Text3DEntry({ waterRadius + bigLineLength,0,0 }, { 0,0,1 }, { 1,0,0 }, "SE"));
		s.createAtlas(smallLineLength);
		s.addCenteredText(Text3DEntry({ waterRadius + bigLineLength,0,0 }, { 0,0,1 }, { 1,0,0 }, "S"));
		sw.createAtlas(smallLineLength);
		sw.addCenteredText(Text3DEntry({ waterRadius + bigLineLength,0,0 }, { 0,0,1 }, { 1,0,0 }, "SW"));
		w.createAtlas(smallLineLength);
		w.addCenteredText(Text3DEntry({ waterRadius + bigLineLength,0,0 }, { 0,0,1 }, { 1,0,0 }, "W"));
		nw.createAtlas(smallLineLength);
		nw.addCenteredText(Text3DEntry({ waterRadius + bigLineLength,0,0 }, { 0,0,1 }, { 1,0,0 }, "NW"));

		int currentAngle = 0;
		angles.reserve(anglesCount);
		for (size_t i = 0; i < anglesCount; i++)
		{
			angles.emplace_back();

			angles[i].createAtlas(0.1);
			angles[i].addText(Text3DEntry({ waterRadius + mediumLineLength,0,bigLineWidth }, { 0,0,1 }, { 1,0,0 }, currentAngle, "º"));
			currentAngle += anglesStep;
		}

	}

	void createLinesPolygons()
	{
		//small
		int nLines = 360;
		float step = 360.0f / nLines;
		float angle = 0;//initial value
		for (size_t i = 0; i < nLines; i++)
		{
			vector<p3> rotatedPositions = positionsSmall;
			rotate3D(rotatedPositions, 0, angle, 0);
			compassLines.addPositions(rotatedPositions);

			angle += step;
		}

		//small medium
		nLines = 360 / 5;
		step = 360.0f / nLines;
		angle = 0;//initial value
		for (size_t i = 0; i < nLines; i++)
		{
			vector<p3> rotatedPositions = positionsSM;
			rotate3D(rotatedPositions, 0, angle, 0);
			compassLines.addPositions(rotatedPositions);

			angle += step;
		}

		////medium 10º
		nLines = 360/10;
		step = 360.0f / nLines;
		angle = 0;
		for (size_t i = 0; i < nLines; i++)
		{
			vector<p3> rotatedPositions = positionsMedium;
			rotate3D(rotatedPositions, 0, angle, 0);
			compassLines.addPositions(rotatedPositions);

			angle += step;
		}

		//big
		nLines = 8;
		step = 360.0f / nLines;
		angle = 0;
		for (size_t i = 0; i < nLines; i++)
		{
			vector<p3> rotatedPositions = positionsBig;
			rotate3D(rotatedPositions, 0, angle, 0);
			compassLines.addPositions(rotatedPositions);

			angle += step;
		}

	}

	void draw(Shader& shader3D, Shader& shaderText3D)
	{
		shader3D.bind();
		transparent();
		shader3D.setUniform("u_Model", identityMatrix);
		shader3D.setUniform("u_fragmentMode", shadeColor);
		shader3D.setUniform("u_Color", 40.0f / 255.0f, 189.9f / 255.0f, 255.0f / 255.0f, 0.6);
		water.draw();


		opaque();
		shader3D.setUniform("u_Color", 1, 1, 1, 1);
		compassLines.draw();

		transparent();
		shaderText3D.bind();
		shaderText3D.setUniform("u_Model", identityMatrix);
		n.draw();

		matrix4x4 modelText = identityMatrix;
		rotate3DModelMatrix(modelText, 315, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		ne.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, 270, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		e.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, 225, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		se.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, 180, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		s.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, 135, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		sw.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, 90, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		w.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, 45, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		nw.draw();

		int currentAngle = 0;
		for (size_t i = 0; i < anglesCount; i++)
		{
			modelText = identityMatrix;
			rotate3DModelMatrix(modelText, -currentAngle, { 0,1,0 });
			shaderText3D.setUniform("u_Model", modelText);
			angles[i].draw();
			currentAngle += anglesStep;
		}
	}
};