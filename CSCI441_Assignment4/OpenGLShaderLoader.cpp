#include "OpenGLShaderLoader.h"
#include <fstream>

using std::ifstream;

OpenGLShaderLoader::OpenGLShaderLoader()
{}


OpenGLShaderLoader::~OpenGLShaderLoader()
{}

OpenGLShaderLoader& OpenGLShaderLoader::getInstance()
{
	static OpenGLShaderLoader instance;

	return instance;
}

GLuint OpenGLShaderLoader::createProgram(string vertexShaderFile, string fragmentShaderFile)
{
	int result = 0;

	// first read in the shader code
	string vertexShaderCode = loadShader(vertexShaderFile);
	string fragmentShaderCode = loadShader(fragmentShaderFile);

	// compile the shaders
	GLuint vrtxShader = createShader(GL_VERTEX_SHADER, vertexShaderCode, "vertex shader");
	GLuint frgtShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderCode, "fragment shader");

	// create the program handle and link the shaders
	GLuint program = glCreateProgram();
	glAttachShader(program, vrtxShader);
	glAttachShader(program, frgtShader);
	glLinkProgram(program);

	/*ERROR CHECK*/
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		string log;
		string err = "[ERR]ShaderLoader: Program failed to link\nLOG:\n";
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &result);

		log.resize(result);
		glGetShaderInfoLog(program, result, 0, &log[0]);
		err += log;

		throw exception(err.c_str());
	}
	/*END ERROR CHECK*/

	return program;
}

void OpenGLShaderLoader::deleteProgram(GLuint programId)
{
	glDeleteProgram(programId);
}

string OpenGLShaderLoader::loadShader(string fileName)
{
	string shaderCode;
	ifstream fileStream(fileName.c_str(), std::ios::in);

	// check if the file exsits
	if (!fileStream.good())
		throw exception("[ERR]ShaderLoader: Unable to read passed file");

	// set the string to be the size of the file
	fileStream.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)fileStream.tellg());
	fileStream.seekg(0, std::ios::beg);
	fileStream.read(&shaderCode[0], shaderCode.size());
	fileStream.close();

	return shaderCode;
}

GLuint OpenGLShaderLoader::createShader(GLenum shaderType, string src, string shaderName)
{
	int result = 0;
	const int size = src.size();
	const char* shaderCode = src.c_str();

	GLuint shader = glCreateShader(shaderType);
	
	glShaderSource(shader, 1, &shaderCode, &size);
	glCompileShader(shader);

	/*ERROR CHECK*/
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		string log;
		string err = "[ERR]ShaderLoader: Shader " + shaderName + " failed to compile\nLOG:\n";
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);

		log.resize(result);
		glGetShaderInfoLog(shader, result, 0, &log[0]);
		err += log;

		throw exception(err.c_str());
	}
	/*END ERROR CHECK*/

	return shader;
}
