#include "Graphics.hpp"


Lines3D::~Lines3D()
{
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);

	positions.clear(); indices.clear();
}

void Lines3D::genBuffers() 
{
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void Lines3D::addSet(const vector<p3>& items)
{
	positions.reserve(positions.size() + items.size());

	positions.insert(positions.end(), items.begin(), items.end());

	createIndices(items);

	isBufferUpdated = true;
}

void Lines3D::createIndices(const vector<p3>& items)
{
	indices.reserve(indices.size() + items.size() * 2);

	for (unsigned int i = 0; i < items.size() - 1; i++)
	{
		indices.insert(indices.end(), { indexOffset + i,indexOffset + i + 1 });
	}

	indexOffset = indices.back() + 1;
}

void Lines3D::draw()
{
	glBindVertexArray(vertexArray);

	if (isBufferUpdated)
	{
		positionsDataSize = positions.size() * sizeof(p3);
		indicesDataSize = indices.size() * sizeof(unsigned int);

		size_t maxDataSize = max(positionsDataSize, indicesDataSize);
		if (maxDataSize > bufferSize)
		{
			bufferSize = maxDataSize * 1.5;

			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, usageHint);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, nullptr, usageHint);
		}

		//glBufferData creates a bigger buffer than necessary but glBufferSubData sends the exact amount of memory
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, positionsDataSize, positions.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indicesDataSize, indices.data());


		isBufferUpdated = false;
	}

	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}


void Lines3D::clear()
{
	positions.clear();
	indices.clear();
	indexOffset = 0;
}