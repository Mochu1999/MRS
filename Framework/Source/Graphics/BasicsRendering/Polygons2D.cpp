#include "Graphics.hpp"

Polygons2D::~Polygons2D() {
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
}

void Polygons2D::genBuffers()
{
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void Polygons2D::addSet(const vector<p2>& positions_)
{
	positions.reserve(positions.size() + positions_.size());
	positions.insert(positions.end(), positions_.begin(), positions_.end());

	createConvexIndices(positions_);

	isBufferUpdated = true;
}

//first vertex triangulates with 2 consecutives vertices
void Polygons2D::createConvexIndices(const vector<p2>& positions_)
{
	indices.reserve(indices.size() + (positions_.size() - 3) * 3);

	//you start in the vertex next to the initial, always triangulating with the vertex next to that one
	//the last vertex is the first one repeated and the one before doesn't have a next vertex to triangulate
	//those are the 3 vertices that are excluded in the reserve
	for (unsigned int i = 1; i < positions_.size() - 2; i++)
	{
		indices.insert(indices.end(), { indexOffset, indexOffset + i, indexOffset + i + 1 });
	}

	indexOffset = indices.back() + 2;//position[indices.back()+1] is positions[indices.front()], must be +2
}

void Polygons2D::addSet(const vector<p2>& positions_, const vector<unsigned int>& indices_)
{
	positions.reserve(positions.size() + positions_.size());
	indices.reserve(indices.size() + indices_.size());

	positions.insert(positions.end(), positions_.begin(), positions_.end());
	indices.insert(indices.end(), indices_.begin(), indices_.end());

	isBufferUpdated = true;
}

//Specific for squares
//Heights must be different
void Polygons2D::createRectangle(const p2 corner1, const p2 corner2)
{
	if (corner1.y == corner2.y) return;

	addSet(createRectanglePositions(corner1,corner2));

	isBufferUpdated = true;
}

void Polygons2D::createRectangle(vector<std::pair<p2, p2>> rectangles)
{
	for (auto& rectangle : rectangles)
	{
		p2 corner1 = rectangle.first;
		p2 corner2 = rectangle.second;

		if (corner1.y == corner2.y) return;

		addSet(createRectanglePositions(corner1, corner2));
	}
}

void Polygons2D::addCircle(const int r, const p2 center, int segments)
{
	addSet(createCirclePositions(r, center, segments));

}

void Polygons2D::addCircle(const int r, const vector<p2> centers, int segments)
{
	if (segments == 0)
		segments = r * 3;

	for (p2 center : centers)
	{
		addSet(createCirclePositions(r, center, segments));
	}
}

void Polygons2D::draw()
{
	glBindVertexArray(vertexArray);

	if (isBufferUpdated)
	{
		positionsDataSize = positions.size() * sizeof(p2);
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

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Polygons2D::clear()
{
	indices.clear();
	positions.clear();
	indexOffset = 0;
}