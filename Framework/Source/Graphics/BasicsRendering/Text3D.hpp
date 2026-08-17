#pragma once

//Automatically includes Text.hpp, access to freetype


//Only difference with Text3DEntry is that it uses p3 instead of p2
struct Text3DEntry
{
	p3 pos;
	p3 right; //reading direction of the words
	p3 up; //up direction of the words
	string text;

	template <typename... Args>
	Text3DEntry(p3 pos_, p3 right_, p3 up_, Args&&... body)
		: pos(pos_), right(normalize3(right_)), up(normalize3(up_))
	{
		std::ostringstream oss;
		(oss << ... << body);
		text = oss.str();
	}
};


//To create Text in the 3D world
struct Text3D
{

	unsigned int vertexArray, vertexBuffer, indexBuffer;
	unsigned int textureAtlasTexture; //OpenGL texture object


	vector<float> positions; // x, y, z, s, t; Contains positions and texture coordinates
	vector<unsigned int> indices;

	vector<p3> textPosition;
	vector<p3> textRight;
	vector<p3> textUp;
	vector<string> textToDraw;

	int indexOffset = 0; //to separate different textToDraws
	bool isBufferUpdated = true;





	//in assets/TextMetrics there are photos of what these metrics mean
	struct GlyphMetrics
	{
		float width, height;
		float bearingX, bearingY;
		float advance;
		float texCoordX0 = 0, texCoordY0 = 0;
		float texCoordX1 = 1, texCoordY1 = 1;
	};

	//stores the glyph metrics of every character in allCharacters
	std::map<char, GlyphMetrics> glyphMetricsMap;




	Text3D()
	{
		genBuffers();
	}

	void genBuffers();

	//--- --- ---
	// Atlas functions
	//--- --- ---

	//Encapsulates initializeFreeType and createAtlasTexture. Handles the needed Freetype objects
	void createAtlas(float fontSize, string glyphPath = "resources/Glyphs/Helvetica/Helvetica.otf");

	//initializes the library, the font size and gets a vector of the all characters that the font provides
	void initializeFreeType(FT_Library& ft, FT_Face& face, string& allCharacters, const std::string& fontPath, const int fontSize);

	//main function that includes the initialization of the texture and the call of storeGlyph to end with the final Atlas
	void createAtlasTexture(FT_Face& face, string& allCharacters, float glyphScale);

	//similar to genBuffers but with the atlas texture, but it needs to know how to big the texture is, so it's called in createAtlasTexture
	void genAtlasTexture(const float atlasWidth, const float atlasHeight);






	//fills the vertex buffer with the final quad positions and with the coordinates of the glyph in the atlas
	void fillVertexBuffer();




	//--- --- ---
	// Add text functions
	//--- --- ---

	////Currently if you use substituteText in a while loop it will be equivalent to using addDynamicText, that can't be right

	//meant to be the initial push for static text. It wont delete previous entries. Accepts vector a single Text3DEntry format
	//line must go inside {} in the call, in the vector call and Dynamic the format is: {{},{}};
	void addText(vector<Text3DEntry> line);
	void addText(Text3DEntry line);

	//the bottom center of the string will be centered (without left bearing of the first glyph)
	void addCenteredText(Text3DEntry line);

	//meant to substitute a single entry
	void substituteText(unsigned int i, Text3DEntry line);

	//fancy function to use when you don't want to change the corner position. Here you call it with (index, oss) instead of (index,{p2,oss})
	template <typename... Args>
	void substituteText(unsigned int i, Args&&... body);

	void addDynamicText(vector<Text3DEntry> line);

	//2 triangles per each quad in the order {0,1,2 , 0,2,3}
	void createIndices(size_t i);

	void draw();

	void clear();

	~Text3D();
};