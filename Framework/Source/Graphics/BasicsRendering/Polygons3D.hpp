//Polyhedra works with full models that can be imported from other formats or created in Polygons3d

//here some functions that were inside of polyhedra to try to add them




//Creates 2D polygons in a 3D space
// Allows for multiple polygons
// Last vertex must be equal to the first one
struct Polygons3D
{
	vector<p3> positions;
	vector <unsigned int> indices;
	vector<p3> normals;


	unsigned int vertexBuffer;
	unsigned int vertexArray;
	unsigned int indexBuffer;
	unsigned int normalsBuffer;


	size_t positionsDataSize = 0;
	size_t indicesDataSize = 0;
	size_t bufferSize = 0;

	bool isBufferUpdated = false;
	GLenum usageHint = GL_DYNAMIC_DRAW;

	unsigned int indexOffset = 0;

	Polygons3D()
	{
		genBuffers();
	}

	~Polygons3D();

	void genBuffers();

	void draw();

	void clear();

	// Last vertex must be equal to the first one
	// Additive
	void addPositions(const vector<p3>& positions_)
	{
		if (positions_.size() >= 3)
		{
			positions.insert(positions.end(), positions_.begin(), positions_.end());
			createConvexIndices(positions_);

			//The polygon shares the same normal on all its vertices
			p3 normal = normalize3(cross3(positions_[1] - positions_[0], positions_[2] - positions_[0]));
			for (int i = 0; i < positions.size(); i++)
				normals.push_back(normal);

			isBufferUpdated = true;
		}
	}

	


	void createConvexIndices(const vector<p3>& positions_)
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
};

