#include "Graphics.hpp"

Lines2DInstanced::~Lines2DInstanced()
{
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &instanceBuffer);

	positions.clear(); indices.clear(); instances.clear();
}

void Lines2DInstanced::genBuffers()
{
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);
	glGenBuffers(1, &instanceBuffer);

	//setting vertexBuffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);


	//setting instanceBuffer
	glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);

	// translation (one vec2)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceAttributes), (void*)0);
	glVertexAttribDivisor(1, 1);  //(index,divisor(how frequently the attribute changes))

	// rotation (vec2 cosTheta, sinTheta) goes to location 2:
	glEnableVertexAttribArray(2);
	// the offset is the size of the translation, a p2
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceAttributes), (void*)(sizeof(p2)));  
	glVertexAttribDivisor(2, 1);

	// scale (vec2) goes to location 3
	glEnableVertexAttribArray(3);
	// offset is the translation plus the rotation (2 floats)
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(InstanceAttributes), (void*)(sizeof(p2) + 2 * sizeof(float)));  
	glVertexAttribDivisor(3, 1);
	glBindVertexArray(0);
}

void Lines2DInstanced::addInitialSet(const vector<p2>& items)
{
	positions.clear();
	indices.clear();

	positions.reserve(items.size());
	indices.reserve(items.size() * 2);

	positions.insert(positions.end(), items.begin(), items.end());

	createIndices(items);
}

void Lines2DInstanced::createIndices(const vector<p2>& items)
{
	for (unsigned int i = 0; i < items.size() - 1; i++)
	{
		indices.insert(indices.end(), { i,i + 1 });
	}
}

void Lines2DInstanced::addInstances(const vector<InstanceAttributes> instances_)
{
	instances.clear();
	instances.reserve(instances_.size());
	instances = instances_;

	isBufferUpdated = true;
}

void Lines2DInstanced::addMoreInstances(const vector<InstanceAttributes> instances_)
{
	instances.reserve(instances.size() + instances_.size());
	instances.insert(instances.end(), instances_.begin(), instances_.end());

	isBufferUpdated = true;
}

void Lines2DInstanced::draw()
{
	glBindVertexArray(vertexArray);

	if (isBufferUpdated)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(p2), positions.data(), usageHint);

		glBindBuffer(GL_ARRAY_BUFFER, instanceBuffer);
		glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(InstanceAttributes), instances.data(), usageHint);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), usageHint);
	}

	glDrawElementsInstanced(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0, instances.size());
}

void Lines2DInstanced::clear()
{
	positions.clear();
	indices.clear();
	instances.clear();
}