#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H



//to do
//There's mo kerning, that what improves spacing between glyphs for specific pairs, like AA vs AV
// texto en dpis, reserves
// Separación de atlas y addText en dos diferentes struct. Que solo haya un objeto en UI que alimente toda instancia de texto
// Un solo atlas con distintos tamaños de font
// Que tu des un tamaño de texto y el struct te elija automáticamente la font
//Al parecer si haces las texturas más grandes tendrás más resolución al final, pero la textura consumirá más memoria

//An entry with a position and an undertermined amount of text arguments that gets added in addText
struct TextEntry
{
	p2 pos;
	string text;

	template <typename... Args>
	TextEntry(p2 pos_, Args&&... body) : pos(pos_)
	{
		std::ostringstream oss;
		(oss << ... << body);
		text = oss.str();
	}
};

//2 different parts, Atlas creation and addTexts.
//Size of text is set in pixels to maintain proportions between devices
struct Text
{

	unsigned int vertexArray, vertexBuffer, indexBuffer;
	unsigned int textureAtlasTexture; //OpenGL texture object


	vector<float> positions; //Contains positions and texture coordinates
	vector<unsigned int> indices;

	vector<p2> textPosition;
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




	Text()
	{
		genBuffers();
	}

	void genBuffers();

	//--- --- ---
	// Atlas functions
	//--- --- ---

	//Encapsulates initializeFreeType and createAtlasTexture. Handles the needed Freetype objects
	void createAtlas(int fontPixelSize, string glyphPath = "resources/Glyphs/Helvetica/Helvetica.otf");

	//initializes the library, the font size and gets a vector of the all characters that the font provides
	void initializeFreeType(FT_Library& ft, FT_Face& face, string& allCharacters, const std::string& fontPath, const int fontPixelSize);

	//main function that includes the initialization of the texture and the call of storeGlyph to end with the final Atlas
	void createAtlasTexture(FT_Face& face, string& allCharacters);

	//similar to genBuffers but with the atlas texture, but it needs to know how to big the texture is, so it's called in createAtlasTexture
	void genAtlasTexture(const float atlasWidth, const float atlasHeight);






	//fills the vertex buffer with the final quad positions and with the coordinates of the glyph in the atlas
	void fillVertexBuffer();




	//--- --- ---
	// Add text functions
	//--- --- ---

	////Currently if you use substituteText in a while loop it will be equivalent to using addDynamicText, that can't be right

	//meant to be the initial push for static text. It wont delete previous entries. Accepts vector a single TextEntry format
	//line must go inside {} in the call, in the vector call and Dynamic the format is: {{},{}};
	void addText(vector<TextEntry> line);
	void addText(TextEntry line);
	void addCenteredText(TextEntry line);

	//meant to substitute a single entry
	void substituteText(unsigned int i, TextEntry line);

	//fancy function to use when you don't want to change the corner position. Here you call it with (index, oss) instead of (index,{p2,oss})
	template <typename... Args>
	void substituteText(unsigned int i, Args&&... body);

	void addDynamicText(vector<TextEntry> line);

	void createIndices(size_t i);

	void draw();

	//incompleto
	void clear()
	{
		textPosition.clear();
		textToDraw.clear();
	}

	~Text();
};

/* Usage example
text.createAtlas(16, "resources/Glyphs/Helvetica/Helvetica.otf");

TextEntry textEntry({ 50,100}, "algo");
text.addDynamicText({ textEntry });
text.draw();
*/



//--- --- ---
// String format functions
//--- --- ---

template<typename T>
inline T round2d(T number) {
	return round(number * 100.0) / 100.0;
}

template<typename T>
inline T round1d(T number) {
	return round(number * 10.0) / 10.0;
}