#include "Graphics.hpp"

Polygons3D::~Polygons3D() {
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
}

void Polygons3D::genBuffers()
{
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);
	glGenBuffers(1, &normalsBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void Polygons3D::clear()
{
	positions.clear();
	indices.clear();
	normals.clear();

	indexOffset = 0;
}

void Polygons3D::draw()
{
	glBindVertexArray(vertexArray);

	if (isBufferUpdated)
	{
		positionsDataSize = positions.size() * sizeof(p3);
		indicesDataSize = indices.size() * sizeof(unsigned int);
		//Assumed that normals size is the same as positions

		size_t maxDataSize = max(positionsDataSize, indicesDataSize);
		if (maxDataSize > bufferSize)
		{
			bufferSize = maxDataSize * 1.5;

			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, usageHint);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, nullptr, usageHint);

			glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
			glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, usageHint);
		}

		//glBufferData creates a bigger buffer than necessary but glBufferSubData sends the exact amount of memory
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, positionsDataSize, positions.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indicesDataSize, indices.data());

		glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, positionsDataSize, normals.data());

		isBufferUpdated = false;
	}

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

		
