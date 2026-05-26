#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H



//to do
//There's mo kerning, that what improves spacing between glyphs for specific pairs, like AA vs AV
// texto en dpis, reserves
// Separación de atlas y addText en dos diferentes struct. Que solo haya un objeto en UI que alimente toda instancia de texto
// Un atlas con distintos tamaños de font
// Que tu des un tamaño de texto y el struct te elija automáticamente la font

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


	//in assets/TextMetrics there are photos of what these metrics mean
	struct GlyphMetrics
	{
		float width, height;
		float bearingX, bearingY;
		float advance;
		float texCoordX0 = 0, texCoordY0 = 0;
		float texCoordX1 = 1, texCoordY1 = 1;
	};

	//NO SÉ SI SE PUEDE ENCAPSULAR
	//stores the glyph metrics of every character in allCharacters
	std::map<char, GlyphMetrics> glyphMetricsMap;





	vector<float> positions; //CONTIENE POSITIONES Y TEXTURE COORDINATES
	vector<unsigned int> indices;
	
	vector<p2> textPosition;
	vector<string> textToDraw;

	int indexOffset = 0; //to separate different textToDraws


	
	



	bool isBufferUpdated = true;


	Text() 
	{
		genBuffers();
	}

	void genBuffers();

	//--- --- ---
	// Atlas functions
	//--- --- ---

	//initializes FreeType and creates the atlas
	void createAtlas(string glyphPath, int fontPixelSize)
	{
		//Freetype objects are discarded after creating the atlas
		//ft is the FreeType context, the internal state the library needs to operate. Needed to create our use FT_Face
		FT_Library ft;
		FT_Face face; //stores a font //you can have multiple fonts in one ft
		//Font face would be Helvetica Bold Italic, while a typeface would be helvetica. Here we are talking about a font face

		//A font is a collection of characters and glyphs, being glyphs the geometric representation that will be drawn
		//allCharacters stores all the used unicode values (their associated characters really) so their glpyh data is retrieved in storeGlyph
		string allCharacters;


		initializeFreeType(ft,face, allCharacters, glyphPath, fontPixelSize);
		createAtlasTexture(face, allCharacters);

		// Cleanup FreeType resources //I'm not sure they aren't deleted out of scope
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}

	

	//initializes the library, the font size and gets a vector of the all characters that the font provides
	void initializeFreeType(FT_Library& ft, FT_Face& face, string& allCharacters,const std::string& fontPath, const int fontPixelSize);

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
	void addText(vector<TextEntry> line)
	{
		for (auto& l : line)
		{
			textPosition.push_back(l.pos);
			textToDraw.push_back(l.text);
		}

		fillVertexBuffer();
	}
	//line must go inside {} in the call, in the vector call and Dynamic the format is: {{},{}};
	void addText(TextEntry line)
	{

		textPosition.push_back(line.pos);
		textToDraw.push_back(line.text);


		fillVertexBuffer();
		isBufferUpdated = true;

	}

	//meant to substitute a single entry
	void substituteText(unsigned int i, TextEntry line)
	{

		textPosition[i] = line.pos;
		textToDraw[i] = line.text;


		fillVertexBuffer();
		isBufferUpdated = true;
	}
	//fancy function if you don't want to change the position. Here you call it with (index, oss) instead of (index,{p2,oss})
	template <typename... Args>
	void substituteText(unsigned int i, Args&&... body)
	{
		std::ostringstream oss;
		(oss << ... << body);

		textToDraw[i] = oss.str();

		fillVertexBuffer();
		isBufferUpdated = true;

	}

	void addDynamicText(vector<TextEntry> line)
	{
		textPosition.clear();
		textToDraw.clear();

		for (auto& l : line)
		{
			textPosition.push_back(l.pos);
			if (!l.text.empty())
				textToDraw.push_back(l.text);
			else
				textToDraw.push_back(" ");
		}

		fillVertexBuffer();
		isBufferUpdated = true;

	}
	
	void createIndices(size_t i);


	






	void draw() 
	{

		if (isBufferUpdated) 
		{
			glBindVertexArray(vertexArray);
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), /*positions.data()*/ nullptr, GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, indices.data(), GL_DYNAMIC_DRAW);

			glBindTexture(GL_TEXTURE_2D, textureAtlasTexture);
			glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * 4, positions.data());

			isBufferUpdated = false;
		}
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	}


	~Text();

};

/* Usage example
text.createAtlas("resources/Glyphs/Helvetica/Helvetica.otf", 16);

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