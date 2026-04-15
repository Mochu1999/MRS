#pragma once

struct TelemetryUI
{
	Shader& shader3D;
	Shader& shader2D;
	Shader& shader2D_Instanced;
	Shader& shaderText;
	Camera& camera;
	GlobalVariables& gv;
	TimeStruct& tm;

	Lines2D centerCross;
	Overlay2D overlay;
	Graphic graphic;
	Graphic graphic2;
	ProgressBar pb;

	Sphere sun;

	float pbValue = 0.5f;

	Lourdes& lourdes;

	TelemetryUI(Shader& shader3D_, Shader& shader2D_, Shader& shader2D_Instanced_, Shader& shaderText_, Camera& camera_, GlobalVariables& gv_, TimeStruct& tm_, Lourdes& lourdes_)
		:shader3D(shader3D_), shader2D(shader2D_), shader2D_Instanced(shader2D_Instanced_), shaderText(shaderText_), camera(camera_), gv(gv_), tm(tm_), lourdes(lourdes_)
		, overlay(shader2D, camera)
		, graphic(shader2D, shader2D_Instanced, shaderText, camera, tm, "A*cos(x)", { 1400,100 }, graf1Val)
		, graphic2(shader2D, shader2D_Instanced, shaderText, camera, tm, "rudderAngle", { 1400,400 }, lourdes_.rudderAngle)
		, pb(shader2D, shader2D_Instanced, shaderText, camera, tm, p2{ 1350,700 }, "Battery", pbValue)
		, sun(3)
	{
		centerCross.addSet({
			{gv.centerWindow.x - 20,gv.centerWindow.y},{gv.centerWindow.x + 20,gv.centerWindow.y},
			{gv.centerWindow.x,gv.centerWindow.y - 20},{gv.centerWindow.x,gv.centerWindow.y + 20} });
		centerCross.indices = { 0,1,2,3 };
	}

	void draw()
	{
		shader3D.bind();
		shader3D.setUniform("u_Model", identityMatrix);
		shader3D.setUniform("u_fragmentMode", 1);
		sun.draw();

		opaque();

		shader2D.bind();

		overlay.draw();

		graf1Val = cosPlot(c);
		graphic.draw();
		graphic2.draw();
		pb.draw();

		transparent();
		shader2D.bind();
		shader2D.setUniform("u_Model", identityMatrix);
		shader2D.setUniform("u_Color", 1, 1, 1, 0.5);
		glLineWidth(2);
		centerCross.draw();
		glLineWidth(1);
		opaque();
	}
};