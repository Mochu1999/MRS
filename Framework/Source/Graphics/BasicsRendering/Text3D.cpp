#include "Graphics.hpp"


void Text3D::genBuffers()
{
	glBindVertexArray(0);
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	//The are 2 attributes in the vertex bufex: positions and texture coordinates
	//offset is 3 because texture comes after 3 floats; stride is the next to last element: 5 floats
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); //still has 2 components, starts after the third

	glBindVertexArray(0);
}

void Text3D::createAtlas(float fontSize, string glyphPath)
{
	int rasterSize = 256;
	float glyphScale = fontSize / static_cast<float>(rasterSize);

	//Freetype objects are discarded after creating the atlas
	//ft is the FreeType context, the internal state the library needs to operate. Needed to create our use FT_Face
	FT_Library ft;
	FT_Face face; //stores a font //you can have multiple fonts in one ft
	//Font face would be Helvetica Bold Italic, while a typeface would be helvetica. Here we are talking about a font face

	//A font is a collection of characters and glyphs, being glyphs the geometric representation that will be drawn
	//allCharacters stores all the used unicode values (their associated characters really) so their glpyh data is retrieved in storeGlyph
	string allCharacters;


	initializeFreeType(ft, face, allCharacters, glyphPath, rasterSize);
	createAtlasTexture(face, allCharacters, glyphScale);

	// Cleanup FreeType resources //I'm not sure they aren't deleted out of scope
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Text3D::initializeFreeType(FT_Library& ft, FT_Face& face, string& allCharacters, const std::string& fontPath, int rasterSize)
{
	//initializes the FreeType library and links the created library context into ft
	FT_Init_FreeType(&ft);

	FT_New_Face(ft, fontPath.c_str(), 0, &face);
	//The 0 is there if the .ttf or .otf contain a typeface family, and the index let's you choose a single font face

	// Set font size in pixels //Direct alternative to dpis
	FT_Set_Pixel_Sizes(face, 0, rasterSize);


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


void Text3D::createAtlasTexture(FT_Face& face, string& allCharacters, float glyphScale)
{
	constexpr int atlasPadding = 2; //transparent pixels around each glyph to avoid texture bleeding
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


		//glyphMetricsMap.emplace(c, GlyphMetrics{ width, height, bearingX, bearingY, advance });

		//atlasWidth += width;

		//if (glyphMetricsMap[c].height > atlasHeight)
		//	atlasHeight = glyphMetricsMap[c].height; //highest glyph determines final atlasHeight
		glyphMetricsMap.emplace(c, GlyphMetrics{ width * glyphScale, height * glyphScale, bearingX * glyphScale, bearingY * glyphScale, advance * glyphScale });

		atlasWidth += width + 2 * atlasPadding;

		if (height + 2 * atlasPadding > atlasHeight)
			atlasHeight = height + 2 * atlasPadding;
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
		int glyphX = static_cast<int>(currentWidth) + atlasPadding;
		int glyphY = atlasPadding;

		glTexSubImage2D(
			GL_TEXTURE_2D,
			0, //level of detail, setted on base detail
			glyphX, //X offset in the texture where the subimage will be placed
			glyphY, //Y offset in the texture
			bitmap.width, // width of the subimage (glyph)
			bitmap.rows, //  height of the subimage
			GL_RED,
			GL_UNSIGNED_BYTE,
			bitmap.buffer // The actual bitmap data for the glyph.
		);


		//height is the total height, width is the total width of the glyph, advance is the width plus the space till next glyph
		glyphMetricsMap[c].texCoordX0 = glyphX / atlasWidth;
		/*glyphMetricsMap[c].texCoordX1 = (currentWidth + glyphMetricsMap[c].width) / atlasWidth;
		glyphMetricsMap[c].texCoordY0 = 0;
		glyphMetricsMap[c].texCoordY1 = glyphMetricsMap[c].height / atlasHeight;

		currentWidth += glyphMetricsMap[c].width;*/
		glyphMetricsMap[c].texCoordX1 = (glyphX + bitmap.width) / atlasWidth;
		glyphMetricsMap[c].texCoordY0 = glyphY / atlasHeight;
		glyphMetricsMap[c].texCoordY1 = (glyphY + bitmap.rows) / atlasHeight;

		currentWidth += bitmap.width + 2 * atlasPadding;
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


void Text3D::genAtlasTexture(const float atlasWidth, const float atlasHeight)
{
	glGenTextures(1, &textureAtlasTexture);
	glBindTexture(GL_TEXTURE_2D, textureAtlasTexture);

	//The atlas must start transparent. Otherwise the padding pixels contain undefined data.
	std::vector<unsigned char> emptyAtlas(
		static_cast<size_t>(atlasWidth) * static_cast<size_t>(atlasHeight), 0);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,//level of detail, set on base detail
		GL_RED,	//this gives the internal format, look for more info
		atlasWidth, // width of the entire texture atlas.
		atlasHeight, //height of the tallest glyph in the atlas, used as the atlas height.
		0, //border of the texture, must be 0
		GL_RED,
		GL_UNSIGNED_BYTE,
		emptyAtlas.data());



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
void Text3D::fillVertexBuffer() {
	positions.clear();
	indices.clear();
	indexOffset = 0;


	for (size_t i = 0; i < textToDraw.size(); i++)
	{
		//bottom coordinates of the string to render
		p3 origin = textPosition[i];
		p3 right = textRight[i];
		p3 up = textUp[i];

		//variable that represents the current right offset
		float x = 0.0f;

		for (size_t j = 0; j < textToDraw[i].size(); ++j) {

			char c = textToDraw[i][j];
			GlyphMetrics metrics = glyphMetricsMap[c];


			float x0 = x + metrics.bearingX;
			float y0 = -metrics.height + metrics.bearingY;
			float x1 = x0 + metrics.width;
			float y1 = y0 + metrics.height;

			p3 v0 = origin + right * x0 + up * y0;
			p3 v1 = origin + right * x1 + up * y0;
			p3 v2 = origin + right * x1 + up * y1;
			p3 v3 = origin + right * x0 + up * y1;


			float s0 = metrics.texCoordX0;
			float t0 = metrics.texCoordY0;
			float s1 = metrics.texCoordX1;
			float t1 = metrics.texCoordY1;

			//positions and texture coordinates interleaved
			positions.insert(positions.end(),
				{
					v0.x, v0.y, v0.z, s0, t1,
					v1.x, v1.y, v1.z, s1, t1,
					v2.x, v2.y, v2.z, s1, t0,
					v3.x, v3.y, v3.z, s0, t0
				});


			x += metrics.advance;

			createIndices(j);
		}
		if (!indices.empty())
			indexOffset = indices.back() + 1;
	}
}

void Text3D::addText(vector<Text3DEntry> line)
{
	if (line.size() == 0)
		return;

	for (auto& l : line)
	{
		textPosition.push_back(l.pos);
		textRight.push_back(l.right);
		textUp.push_back(l.up);
		textToDraw.push_back(l.text);
	}

	fillVertexBuffer();

	isBufferUpdated = true;
}
//line must go inside {} in the call, in the vector call and Dynamic the format is: {{},{}};
void Text3D::addText(Text3DEntry line)
{
	if (line.text.empty())
		return;

	textPosition.push_back(line.pos);
	textRight.push_back(line.right);
	textUp.push_back(line.up);
	textToDraw.push_back(line.text);


	fillVertexBuffer();

	isBufferUpdated = true;

}

void Text3D::addCenteredText(Text3DEntry line)
{
	if (line.text.empty())
		return;

	//needs to substract the metrics of our string before doing an addText in the correct place
	float x = 0.0f;
	float left = 0.0f;
	float right = 0.0f;

	//getting the metrics of each glyph of the string
	for (size_t i = 0; i < line.text.size(); i++)
	{
		GlyphMetrics metrics = glyphMetricsMap[line.text[i]];

		float x0 = x + metrics.bearingX;
		float x1 = x0 + metrics.width;

		//if we are in the first letter, we substract the left bearing
		if (i == 0)
		{
			left = x0;
		}

		right = x1;

		x += metrics.advance;
	}

	float center = (left + right) / 2.0f;

	//corrected TextEntry position
	line.pos -= line.right * center;

	addText(line);
}

void Text3D::substituteText(unsigned int i, Text3DEntry line)
{

	textPosition[i] = line.pos;
	textRight[i] = line.right;
	textUp[i] = line.up;
	textToDraw[i] = line.text;


	fillVertexBuffer();
	isBufferUpdated = true;
}

void Text3D::addDynamicText(vector<Text3DEntry> line)
{
	clear();

	for (auto& l : line)
	{
		textPosition.push_back(l.pos);
		textRight.push_back(l.right);
		textUp.push_back(l.up);

		if (!l.text.empty())
			textToDraw.push_back(l.text);
		else
			textToDraw.push_back(" ");
	}

	fillVertexBuffer();
	isBufferUpdated = true;

}

void Text3D::createIndices(size_t i) {
	unsigned int aux = i * 4;
	indices.insert(indices.end(), { indexOffset + aux,indexOffset + aux + 1,indexOffset + aux + 2,indexOffset + aux,indexOffset + aux + 2,indexOffset + aux + 3 });
}

template <typename... Args>
void Text3D::substituteText(unsigned int i, Args&&... body)
{
	std::ostringstream oss;
	(oss << ... << body);

	textToDraw[i] = oss.str();

	fillVertexBuffer();
	isBufferUpdated = true;

}

void Text3D::draw()
{
	glBindVertexArray(vertexArray);

	if (isBufferUpdated)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

		isBufferUpdated = false;
	}

	glActiveTexture(GL_TEXTURE0); //textureAtlasTexture will be bound to texture unit 0, texture bindings affect only that slot
	glBindTexture(GL_TEXTURE_2D, textureAtlasTexture);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Text3D::clear()
{
	textPosition.clear();
	textRight.clear();
	textUp.clear();
	textToDraw.clear();

	positions.clear();
	indices.clear();
	indexOffset = 0;
	isBufferUpdated = true;
}

Text3D::~Text3D()
{
	clear();

	// Clean up the resources
	glDeleteTextures(1, &textureAtlasTexture);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer); // Ensure you've created and stored the indexBuffer handle
	glDeleteVertexArrays(1, &vertexArray);
}