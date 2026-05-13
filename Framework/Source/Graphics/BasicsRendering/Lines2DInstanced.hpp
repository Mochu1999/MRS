#pragma once

//outside the main struct to manage vectors of InstanceAttributes wherever
struct InstanceAttributes
{
	p2 translation;
	float cosTheta;
	float sinTheta;
	p2 scale;

	//In the shader lines are first scaled, then rotated, then translated
	// and so they expect the same format in the constructor
	InstanceAttributes(p2 scale_, float angle_, p2 translation_)
		:scale(scale_), cosTheta(cos(angle_)), sinTheta(sin(angle_)), translation(translation_)
	{
	}
};


//You set one initial batch of lines, and multiple batches will come out of it with added translations, rotations and scalings
//Have in mind that rotations happen from O, so normally base positions should be in O
struct Lines2DInstanced
{
	vector<p2> positions;
	vector <unsigned int> indices;
	vector<InstanceAttributes> instances;

	unsigned int vertexArray;
	unsigned int vertexBuffer;
	unsigned int indexBuffer;
	unsigned int instanceBuffer;

	bool isBufferUpdated = false;
	GLenum usageHint = GL_DYNAMIC_DRAW;

	Lines2DInstanced()
	{
		genBuffers();
	}

	~Lines2DInstanced();

	void genBuffers();

	//The one that is getting translated, rotated and scaled
	void addInitialSet(const vector<p2>& items);

	void createIndices(const vector<p2>& items);

	//Substitutive, it will overwrite past addInstances
	void addInstances(const vector<InstanceAttributes> instances_);

	//Non substitutive
	void addMoreInstances(const vector<InstanceAttributes> instances_);

	void draw();

	void clear();
};

/*usage example
* 
Lines2DInstanced lines;
lines.addInitialSet({ {0,0},{300,0}, {300,100}, {0,100}, {0,0} });
lines.addInstances({ { { 1,1 },0, {100,100} } });
lines.addMoreInstances({ {{1,1},radians(30),{100,100} }, { { 2,0.2 },radians(-90),{500,800}} });

shader2DInstanced.bind();
shader2DInstanced.setUniform("u_Color", 1, 1, 1, 1);
lines.draw();

*/