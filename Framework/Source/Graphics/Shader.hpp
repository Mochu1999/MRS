#pragma once

struct Shader 
{
	struct ShaderProgramSource 
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	//OpenGL program handle identifier, returned by glCreateProgram()
	//Each program contains a vertex shader and a fragment shader
	//id is only use internally to call the program
	unsigned int id;

	

	Shader(const std::string& filepath)
		:id(0) 
	{
		ShaderProgramSource source = parseShader(filepath);
		id = createShader(source.VertexSource, source.FragmentSource);
	}
	~Shader();

	//Converts a .shader text into 2 separate strings
	ShaderProgramSource parseShader(const std::string& filepath);

	//Compiles both shaders into a compiled shader object in the GPU
	unsigned int compileShader(unsigned int type, const std::string& source);

	//Combines compiled shaders into a program
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	
	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char* type);

	//Tells openGL what program to use (or to stop using)
	void bind();
	void unbind();



	//locations are the identifiers for uniforms
	//These identifiers are stored with internal ints, but we associate a string to it to call it more comfortably
	//The idea is that one does a setUniform() setting a uniform name and a value

	std::unordered_map<std::string, int> locationsMap;
	//It links the uniform string with its actual id. If the location isn't yet created in locationsMap it creates it
	int getUniformLocation(const std::string& name);

	//Some locations are passed by reference for efficiency, openGL doesn't update itself if the reference later changes of value

	//1f
	void setUniform(const std::string& name, float value);

	//3f
	void setUniform(const std::string& name, float x, float y, float z);

	//p3
	void setUniform(const std::string& name, const p3& vec);
	
	//4f
	void setUniform(const std::string& name, float x, float y, float z, float w);

	//1i
	void setUniform(const std::string& name, int value);

	//mat4
	void setUniform(const std::string& name, const std::array<float, 16>& mat);

};


//#define CHECK_GL_ERROR() {\
//    GLenum err = glGetError();\
//    while (err != GL_NO_ERROR) {\
//        std::cout << "OpenGL error: " << std::hex << err << std::dec << " line:" << __LINE__ << std::endl;\
//        err = glGetError();\
//    }\
//}
//std::set<std::string> printedErrors;
//
//void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) { //Only adds errors once
//	std::string errorMsg(message);
//
//	if (printedErrors.find(errorMsg) == printedErrors.end()) {
//		std::cerr << message << std::endl;
//
//		printedErrors.insert(errorMsg);
//	}
//}