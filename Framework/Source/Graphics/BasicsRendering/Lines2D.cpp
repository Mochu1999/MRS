#include "Graphics.hpp"

void Lines2D::genBuffers()
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

void Lines2D::addSet(const vector<p2>& items)
{
	positions.reserve(positions.size() + items.size());

	positions.insert(positions.end(), items.begin(), items.end());

	createIndices(items);

	isBufferUpdated = true;
}

void Lines2D::createIndices(const vector<p2>& items)
{
	indices.reserve(indices.size() + items.size() * 2);

	for (unsigned int i = 0; i < items.size() - 1; i++)
	{
		indices.insert(indices.end(), { indexOffset + i,indexOffset + i + 1 });
	}

	indexOffset = indices.back() + 1;
}

void Lines2D::createRectangle(const p2 corner1, const p2 corner2)
{
	if (corner1.y == corner2.y) return;

	addSet(createRectanglePositions(corner1,corner2));

}

void Lines2D::createRectangle(vector<std::pair<p2,p2>> rectangles)
{
	for(auto& rectangle : rectangles)
	{
		p2 corner1 = rectangle.first;
		p2 corner2 = rectangle.second;

		if (corner1.y == corner2.y) return;

		addSet(createRectanglePositions(corner1, corner2));
	}
}

void Lines2D::createCircle(const int r, const p2 center, int segments)
{
	if (segments == 0)
		segments = r * 3;

	addSet(createCirclePositions(r, center, segments));

}

void Lines2D::createCircle(const int r, const vector<p2> centers, int segments)
{
	if (segments == 0)
		segments = r * 3;

	for (p2 center : centers)
	{
		addSet(createCirclePositions(r, center, segments));
	}
}

void Lines2D::draw()
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

	glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
}



Lines2D::~Lines2D()
{
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);

	positions.clear(); indices.clear();
}


void Lines2D::clear()
{
	positions.clear();
	indices.clear();
	indexOffset = 0;
}


void Lines2D::addDelaunaySet(const vector<p2>& positions_, const vector<unsigned int>& indices_)
{
	clear();
	for (size_t i = 0; i < indices_.size(); i += 3)
	{
		p2 a = positions_[indices_[i]];
		p2 b = positions_[indices_[i + 1]];
		p2 c = positions_[indices_[i + 2]];
		addSet(vector<p2>{a, b, c, a});
	}
}




