#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

using std::string;
using std::exception;

class OpenGLShaderLoader
{
public:
	static OpenGLShaderLoader& getInstance();
	GLuint createProgram(string vertexShaderFile, string fragmentShaderFile);
	void deleteProgram(GLuint programId);

private:
	OpenGLShaderLoader();
	~OpenGLShaderLoader();
	string loadShader(string fileName);
	GLuint createShader(GLenum shaderType, string src, string shaderName);
};

