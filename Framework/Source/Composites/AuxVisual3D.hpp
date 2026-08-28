#pragma once





struct AuxVisual3D
{
	float waterRadius = 2;

	Telemetry& t;

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

	//Lines
	Lines3D headingLine, shipSpeedLine, sailLine, rudderLine;

	AuxVisual3D(Telemetry& t_) :t(t_)
	{

		createWaterMesh();


		createRoseLinesPolygons();

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

		//Lines
		{
			headingLine.addSet({ {0,0,0},{waterRadius + bigLineLength,0,0} });
			shipSpeedLine.addSet({ {0,0,0},{waterRadius + bigLineLength,0,0} });
			sailLine.addSet({ {0,-0.5,0},{0,1.75,0} });
			rudderLine.addSet({ {0,-0.25,0},{0,0.25,0} });

		}

	}

	//creates a rectangle of a fixed length
	void createWaterMesh()
	{
		/*
		{
			Polygons2D rectangle;

			rectangle.addRectangle({ -10,-10 }, { 10,10 });
			vector<p3> positions = p2ToP3Y(rectangle.positions);

			vector<p3> normals;
			for (size_t i = 0; i < positions.size(); i++)
			{
				normals.push_back({ 0,1,0 });
			}

			water.addPolyhedra(positions, normals, rectangle.indices);
		}*/

		//sin(x)
		float width = 10;
		vector<p3> positions, normals;
		vector<unsigned int> indices;
		float waveLength = 1; //m
		float k = 2 * PI / waveLength;
		float step = PI / 100;

		float waveAmplitude = 0.02;

		{
			p3 a = { -10,0,0 };
			p3 b = { -10,0,width };
			p3 c, d; d.z += width;
			positions.insert(positions.end(), { a,b,c, b,d,c });

			p3 normal = { 0,1,0 };
			normals.insert(normals.end(), { normal, normal, normal, normal, normal, normal });

			indices.insert(indices.end(), { 0,1,2 ,3,4,5 });

		}
		for (unsigned int i = 0; i < /*2 * */PI / step; i++)
		{
			float currentX = i * step;
			p3 a = { currentX / k,waveAmplitude * sin(currentX),0 };
			p3 b = a; b.z = width;
			p3 c = { (currentX + step) / k,waveAmplitude * sin(currentX + step),0 };
			p3 d = c; d.z = width;


			positions.insert(positions.end(), { a,b,c, b,d,c });

			//CW to make them positive because reference system is fucked
			p3 normal = cross3(b - a, c - a);
			normals.insert(normals.end(), { normal, normal, normal });

			normal = cross3(d - b, c - b);
			normals.insert(normals.end(), { normal, normal, normal });

			unsigned int currentIndex = i * 6 + 6;
			indices.insert(indices.end(), { currentIndex,currentIndex + 1,currentIndex + 2	,currentIndex + 3,currentIndex + 4,currentIndex + 5 });
		}
		{
			p3 a = positions[positions.size() - 2];
			p3 b = positions[positions.size() - 1];
			p3 c = a; c.x += 10;
			p3 d = b; d.x += 10;
			positions.insert(positions.end(), { a,b,c, b,d,c });

			p3 normal = { 0,1,0 };
			normals.insert(normals.end(), { normal, normal, normal, normal, normal, normal });

			unsigned int li = indices.back() + 1; //last index
			indices.insert(indices.end(), { li,li + 1,li + 2	,li + 3,li + 4,li + 5 });

		}
		water.addPolyhedra(positions, normals, indices);
	}

	void createRoseLinesPolygons()
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
		nLines = 360 / 10;
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

	float waterPos = 2.2;

	void draw(Shader& shader3D, Shader& shaderText3D, Shader& shaderWater)
	{
		{
			transparent();
			shaderWater.bind();
			shaderWater.setUniform("u_CropRadius", waterRadius);
			matrix4x4 waterMatrix = identityMatrix;
			translate3DModelMatrix(waterMatrix, { waterPos,0,-5 });

			waterPos -= 0.01;
			if (waterPos < -2.2)
				waterPos = 2.2;

			shaderWater.setUniform("u_Model", waterMatrix);

			shaderWater.setUniform("u_fragmentMode", shadeColor);
			shaderWater.setUniform("u_Color", lightBlue, 0.4);
			water.draw();
		}


		shader3D.bind();
		matrix4x4 headingMatrix = identityMatrix;
		rotate3DModelMatrix(headingMatrix, t.headingAngle, { 0,1,0 });




		opaque();
		shader3D.setUniform("u_Color", white, 1);
		matrix4x4 modelText = identityMatrix;
		rotate3DModelMatrix(modelText, t.headingAngle, { 0,1,0 });
		shader3D.setUniform("u_Model", modelText);
		compassLines.draw();


		transparent();
		shaderText3D.bind();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, t.headingAngle, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		n.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, t.headingAngle, { 0,1,0 });
		rotate3DModelMatrix(modelText, 315, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		ne.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, t.headingAngle, { 0,1,0 });
		rotate3DModelMatrix(modelText, 270, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		e.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, t.headingAngle, { 0,1,0 });
		rotate3DModelMatrix(modelText, 225, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		se.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, t.headingAngle, { 0,1,0 });
		rotate3DModelMatrix(modelText, 180, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		s.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, t.headingAngle, { 0,1,0 });
		rotate3DModelMatrix(modelText, 135, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		sw.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, t.headingAngle, { 0,1,0 });
		rotate3DModelMatrix(modelText, 90, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		w.draw();

		modelText = identityMatrix;
		rotate3DModelMatrix(modelText, t.headingAngle, { 0,1,0 });
		rotate3DModelMatrix(modelText, 45, { 0,1,0 });
		shaderText3D.setUniform("u_Model", modelText);
		nw.draw();


		int currentAngle = 0;
		for (size_t i = 0; i < anglesCount; i++)
		{
			modelText = identityMatrix;
			rotate3DModelMatrix(modelText, t.headingAngle, { 0,1,0 });
			rotate3DModelMatrix(modelText, -currentAngle, { 0,1,0 });
			shaderText3D.setUniform("u_Model", modelText);
			angles[i].draw();
			currentAngle += anglesStep;
		}


		shader3D.bind();
		matrix4x4 linesModelMatrix = identityMatrix;

		shader3D.setUniform("u_fragmentMode", 1);

		//sail is already at O
		translate3DModelMatrix(linesModelMatrix, t.shipHeave);
		shader3D.setUniform("u_Model", linesModelMatrix);
		shader3D.setUniform("u_Color", green, 1);
		sailLine.draw();

		translate3DModelMatrix(linesModelMatrix, -t.sailPositionVisual); //shipHeave still applied
		shader3D.setUniform("u_Model", linesModelMatrix);
		rudderLine.draw();

		linesModelMatrix = identityMatrix;
		glLineWidth(3);

		shader3D.setUniform("u_Color", white, 1);
		shader3D.setUniform("u_Model", linesModelMatrix);
		headingLine.draw();

		rotate3DModelMatrix(linesModelMatrix, t.headingAngle, { 0,1,0 });
		shader3D.setUniform("u_Model", linesModelMatrix);
		shader3D.setUniform("u_Color", blue, 1);
		shipSpeedLine.draw();
		glLineWidth(1);
	}
};