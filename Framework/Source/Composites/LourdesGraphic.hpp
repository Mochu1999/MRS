#pragma once



struct LourdesGraphic
{

	Shader& shader3D;
	Camera& camera;
	Polyhedra lightSphere;

	//local matrix we are setting as u_Model
	std::array<float, 16> shipModel3DMatrix = identityMatrix;


	Polyhedra casco, patines, vela, orza, soportes, timon, water;

	float waterLength = 10;

	p3 shipHeave = { 0,-0.1,0 }; float shipHeaveIncrease = 0.0006;
	float rudderAngle = 0; float rudderIncrease = 0.2;

	p3 lightPos = { 30,25,40 };

	LourdesGraphic(Shader& shader3D_, Camera& camera_) :shader3D(shader3D_), camera(camera_)
	{
		/*stlToSimplePolyhedra("casco.stl", "casco.bin");
		stlToSimplePolyhedra("patines.stl", "patines.bin");
		stlToSimplePolyhedra("vela.stl", "vela.bin");
		stlToSimplePolyhedra("timon.stl", "timon.bin");
		stlToSimplePolyhedra("orza.stl", "orza.bin");
		stlToSimplePolyhedra("soportes.stl", "soportes.bin");*/

		casco.addPolyhedra("casco.bin");
		patines.addPolyhedra("patines.bin");
		vela.addPolyhedra("vela.bin");
		orza.addPolyhedra("orza.bin");
		soportes.addPolyhedra("soportes.bin");
		timon.addPolyhedra("timon.bin");



		water.positions = { {0,0,0},{waterLength,0,0},{0,0,waterLength},{waterLength,0,0},{0,0,waterLength},{waterLength,0,waterLength} };
		water.indices = { 0,1,2 ,3,4,5 };
		for (size_t i = 0; i < water.positions.size(); i++)
		{
			water.normals.push_back({ 0,1,0 });
		}
		/*water.normals = { {0,1,-0},{0,1,-0},{0,1,-0},{0,1,-0},{0,1,-0},{0,1,-0} };*/


		lightSphere.addSphere(3,lightPos);
		//lightSphere.addPolyhedra(light.positions, light.normals, light.indices);
		shader3D.bind();
		shader3D.setUniform("u_lightPos", lightPos);

	}

	void draw()
	{
		shader3D.bind();
		shader3D.setUniform("u_Model", identityMatrix);
		shader3D.setUniform("u_fragmentMode", 1);
		lightSphere.draw();

		

		std::array<float, 16> rudderMatrix, propellerMatrix, foilAftMatrix, foilStarboardMatrix, foilPortMatrix, rp, rf;

		if (rudderAngle >= 20 || rudderAngle <= -20)
			rudderIncrease = -rudderIncrease;



		rudderAngle += rudderIncrease;


		//u_model is being always changed to shipModel3DMatrix, but shipModel3DMatrix is only being created if input is changed

		opaque();
		camera.translate3DModelMatrix(shipModel3DMatrix, shipHeave);
		shader3D.setUniform("u_Model", shipModel3DMatrix);


		shader3D.setUniform("u_fragmentMode", 0); //illumination

		shader3D.setUniform("u_Color", 0.1,0.1,0.1, 1.0f);
		casco.draw();
		patines.draw();

		shader3D.setUniform("u_Color", 137.0f / 255.0f, 18.0f / 255.0f, 18.0f / 255.0f, 1.0f);
		vela.draw();
		orza.draw();

		shader3D.setUniform("u_Color", 0.0f / 255.0f, 63.0f / 255.0f, 200.0f / 255.0f, 1.0f);
		soportes.draw();




		//Timon
		{
			shipModel3DMatrix = identityMatrix;

			camera.rotate3DModelMatrix(rudderMatrix, rudderAngle, { 0,1,0 });
			camera.translate3DModelMatrix(rudderMatrix, shipHeave);
			shader3D.setUniform("u_Model", rudderMatrix);


			shader3D.setUniform("u_Color", 113.0f / 255.0f, 10.0f / 255.0f, 87.0f / 255.0f, 1.0f);
			timon.draw();


			shader3D.setUniform("u_Model", identityMatrix);

		}







		transparent();
		shader3D.setUniform("u_Model", identityMatrix);
		camera.translate3DModelMatrix(shipModel3DMatrix, { -waterLength / 2,0,-waterLength / 2 });
		shader3D.setUniform("u_Model", shipModel3DMatrix);

		shader3D.setUniform("u_Color", 40.0f / 255.0f, 189.9f / 255.0f, 255.0f / 255.0f, 0.7);
		water.draw();

		opaque();


		shader3D.setUniform("u_Model", identityMatrix);




	}

	void activateLourdes()
	{
		shader3D.bind();
		shader3D.setUniform("u_lightPos", lightPos);
	}
};