#include "Graphics.hpp"

Polyhedra::~Polyhedra() {
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
}

void Polyhedra::genBuffers()
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

void Polyhedra::addPolyhedra(const std::string& localPath)
{
	clear();

	readSimplePolyhedra(localPath, positions, normals, indices);

	isBufferUpdated = true;
}

void Polyhedra::addPolyhedra(const vector<p3>& positions_, const vector<unsigned int>& indices_, const vector<p3>& normals_)
{
	clear();

	positions = positions_;
	indices = indices_;
	normals = normals_;

	isBufferUpdated = true;
}

void Polyhedra::draw()
{
	glBindVertexArray(vertexArray);

	//No optimizations because it isn't meant to be updated
	if (isBufferUpdated)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(p3), positions.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(p3), normals.data(), GL_DYNAMIC_DRAW);

		isBufferUpdated = false;
	}

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

}

void Polyhedra::clear()
{
	positions.clear();
	indices.clear();
	normals.clear();
}


