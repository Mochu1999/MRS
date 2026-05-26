#include "Graphics.hpp"


void Text::genBuffers() 
{
	glBindVertexArray(0);
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	//The are 2 attributes in the vertex bufex: positions and texture coordinates
	//offset is the last element: 2 floats; stride is the next to last element: 4 floats
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
}


void Text::initializeFreeType(FT_Library& ft, FT_Face& face, string& allCharacters, const std::string& fontPath, const int fontPixelSize)
{
	//initializes the FreeType library and links the created library context into ft
	FT_Init_FreeType(&ft); 

	FT_New_Face(ft, fontPath.c_str(), 0, &face);
	//The 0 is there if the .ttf or .otf contain a typeface family, and the index let's you choose a single font face

	// Set font size in pixels //Direct alternative to dpis
	FT_Set_Pixel_Sizes(face, 0, fontPixelSize);


	//Unicode is the standard that assigns a number to every character. It's fixed: A=65, ñ=241...
	//The font stores a map that links the Unicode values to the glyphs
	// Both variables start unitialized, and FT_Get_First_Char looks for the lowest unicode, normally the first index
	//Then from it, FT_Get_Next_Char will look for the next unicode inmediatly bigger than it, which is normally the next index
	//it doesn't have to be, sometimes the next unicodeValue will be in a previous index
	FT_ULong unicodeValue; 
	FT_UInt glyphIndex;


	unicodeValue = FT_Get_First_Char(face, &glyphIndex);
	while (glyphIndex != 0)
	{
		//128 first characters are ascii characters. For latin-1 characters (ñ, á, ü) you should have the next 128 or maybe just delete the if
		if (unicodeValue < 256)
		{
			allCharacters.push_back(static_cast<char>(unicodeValue));
		}

		unicodeValue = FT_Get_Next_Char(face, unicodeValue, &glyphIndex);

	}

	//std::cout << "Extracted characters: " << allCharacters << std::endl;
}


void Text::createAtlasTexture(FT_Face& face, string& allCharacters)
{
	float atlasWidth = 0, atlasHeight = 0;

	for (char& c : allCharacters)
	{
		//FT_Load_Char has access to the map that links glyph index with its associated unicode value
		// The function renders the glyph into a bitmap from which we extract the metrics
		FT_Load_Char(face, c, FT_LOAD_RENDER);

		FT_GlyphSlot& glyph = face->glyph;
		FT_Bitmap& bitmap = glyph->bitmap; //A bitmap is the FreeType structure that contains the data of a glyph

		//Glyph metrics
		//FreeType explicitly defines bitmap_left and bitmap_top as bitmap bearings
		// Advance is stored in 26.6 fractional pixel format, so >> 6 converts to pixels.
		float bearingX = glyph->bitmap_left;
		float bearingY = glyph->bitmap_top;
		float width = bitmap.width;
		float height = bitmap.rows;
		float advance = glyph->advance.x >> 6;


		glyphMetricsMap.emplace(c, GlyphMetrics{ width, height, bearingX, bearingY, advance });

		atlasWidth += width;

		if (glyphMetricsMap[c].height > atlasHeight)
			atlasHeight = glyphMetricsMap[c].height; //highest glyph determines final atlasHeight
	}

	genAtlasTexture(atlasWidth, atlasHeight);

	//OpenGL by default expects the rows of data to be packed in multiples of 4 bytes, it isn't our case, putting it in 1 somehow means 
	// that “Each row is packed with no extra padding—just one byte per pixel, back-to-back.” and it just works without removing efficiency
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	float currentWidth = 0; //sets the x position of the glyph in the atlas
	for (char& c : allCharacters) { //stores a texture of each glyph in the atlas

		glBindTexture(GL_TEXTURE_2D, textureAtlasTexture);//necessary here

		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Failed to load glyph " << c << std::endl;
			continue;
		}


		//cout << "atlasWidth: " << atlasWidth << " atlasHeight: " << atlasHeight << endl;
		//cout << "face->glyph->bitmap.width: " << face->glyph->bitmap.width << " face->glyph->bitmap.rows: " << face->glyph->bitmap.rows << endl;
		//cout << "currentWidth: " << currentWidth << endl;

		FT_Bitmap& bitmap = face->glyph->bitmap; //A bitmap is the FreeType structure that contains the data of a glyph
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0, //level of detail, setted on base detail
			currentWidth, //X offset in the texture where the subimage will be placed
			0, //Y offset in the texture
			bitmap.width, // width of the subimage (glyph)
			bitmap.rows, //  height of the subimage
			GL_RED,
			GL_UNSIGNED_BYTE,
			bitmap.buffer // The actual bitmap data for the glyph.
		);


		//height is the total height, width is the total width of the glyph, advance is the width plus the space till next glyph
		glyphMetricsMap[c].texCoordX0 = currentWidth / atlasWidth;
		glyphMetricsMap[c].texCoordX1 = (currentWidth + glyphMetricsMap[c].width) / atlasWidth;
		glyphMetricsMap[c].texCoordY0 = 0;
		glyphMetricsMap[c].texCoordY1 = glyphMetricsMap[c].height / atlasHeight;

		currentWidth += glyphMetricsMap[c].width;
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); //Returning to default for next operations

	/*for (auto& pair : glyphMetricsMap) {

		char i = pair.first;
		cout << "Glyph: " << i << endl;
		print(glyphMetricsMap[i].width);
		print(glyphMetricsMap[i].height);
		print(glyphMetricsMap[i].bearingX);
		print(glyphMetricsMap[i].bearingY);
		print(glyphMetricsMap[i].advance);
		print(glyphMetricsMap[i].texCoordX0);
		print(glyphMetricsMap[i].texCoordY0);
		print(glyphMetricsMap[i].texCoordX1);
		print(glyphMetricsMap[i].texCoordY1);
		cout << endl << endl;
	}*/

	glBindTexture(GL_TEXTURE_2D, 0);

	//glyphMetricsMap.clear(); //Clears that memory bc it's not going to be used anymore
}


void Text::genAtlasTexture(const float atlasWidth, const float atlasHeight) 
{
	glGenTextures(1, &textureAtlasTexture);
	glBindTexture(GL_TEXTURE_2D, textureAtlasTexture);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,//level of detail, set on base detail
		GL_RED,	//this gives the internal format, look for more info
		atlasWidth, // width of the entire texture atlas.
		atlasHeight, //height of the tallest glyph in the atlas, used as the atlas height.
		0, //border of the texture, must be 0
		GL_RED,
		GL_UNSIGNED_BYTE,
		nullptr);

	

	//clamps texture outside 0,1 range? Don't think I need it 
		//Revision, after months a bug that shows lines outlines have appeared and the clamp is needed 2025
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/ //linearly interpolates the pixel, smoother, but somewhat blurry 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //choose the nearest pixel: pixelated, but sharp 

	glBindTexture(GL_TEXTURE_2D, 0);
}








//fills the vertex buffer with the final quad positions and the atlas coordinates of the glyph
void Text::fillVertexBuffer() {
	positions.clear();
	indices.clear();
	indexOffset = 0;


	for (size_t i = 0; i < textToDraw.size(); i++)
	{
		//bottom left coordinates of the string to render
		int x = textPosition[i].x;
		int y = textPosition[i].y;

		for (size_t j = 0; j < textToDraw[i].size(); ++j) {

			char c = textToDraw[i][j];
			GlyphMetrics metrics = glyphMetricsMap[c];

			//logic is sound
			float x0 = x + metrics.bearingX;
			float y0 = y - metrics.height + metrics.bearingY;
			float x1 = x0 + metrics.width;
			float y1 = y0 + metrics.height;


			float s0 = metrics.texCoordX0;
			float t0 = metrics.texCoordY0;
			float s1 = metrics.texCoordX1;
			float t1 = metrics.texCoordY1;


			positions.insert(positions.end(), //positions and texture coordinates interleaved
				{ x0, y0, s0, t1,
				x1, y0, s1, t1,
				x1, y1, s1, t0,
				x0, y1, s0, t0
				});


			x += metrics.advance;

			createIndices(j);
		}
		indexOffset = indices.back() + 1;
	}
}

//2 triangles per each quad in the order {0,1,2 , 0,2,3}
void Text::createIndices(size_t i) {
	unsigned int aux = i * 4;
	indices.insert(indices.end(), { indexOffset + aux,indexOffset + aux + 1,indexOffset + aux + 2,indexOffset + aux,indexOffset + aux + 2,indexOffset + aux + 3 });
}




Text::~Text() {
	// Clean up the resources
	glDeleteTextures(1, &textureAtlasTexture);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer); // Ensure you've created and stored the indexBuffer handle
	glDeleteVertexArrays(1, &vertexArray);

	
	
	
}