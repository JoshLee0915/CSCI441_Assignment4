#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLShaderLoader.h"

// window size
#define WIDTH 800
#define HEIGHT 800

// delay const for 60fps
#define FPS 60
#define FPS_DELAY (1/FPS)*1000

// control sensitvity
#define ROT_SENSITVITY 1.0

// camera/light Zpos
#define Z_POS 5.0

using std::cout;

using glm::vec3;
using glm::mat4;
using glm::lookAt;
using glm::perspective;

void init();
void renderScene();
void renderTimer(int val);
void keyPress(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void mousePosTracker(int x, int y);
void drawCube(vec3 scale, const mat4& rot, bool solid);

GLuint solidCubeProg;
GLuint transparentCubeProg;
mat4 proj;

// key press buffer
bool keyStates[256];

// light pos
float lightX, lightY;

int main(int argc, char* args[])
{
	// create the window
	glutInit(&argc, args);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Opengl Assignment 4");
	glewInit();

	init();

	glutDisplayFunc(renderScene);
	glutTimerFunc(FPS_DELAY, renderTimer, 0);
	glutPassiveMotionFunc(mousePosTracker);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyUp);
	glutMainLoop();

	// clean up
	OpenGLShaderLoader::getInstance().deleteProgram(solidCubeProg);
	OpenGLShaderLoader::getInstance().deleteProgram(transparentCubeProg);

	return 0;
}

void renderScene()
{
	static float xRot = 0.0;
	static float yRot = 0.0;
	static float zRot = 0.0;

	// check for key press
	if (keyStates['w'])
		xRot += ROT_SENSITVITY;
	if (keyStates['s'])
		xRot -= ROT_SENSITVITY;
	if (keyStates['d'])
		yRot += ROT_SENSITVITY;
	if (keyStates['a'])
		yRot -= ROT_SENSITVITY;
	if (keyStates['q'])
		zRot += ROT_SENSITVITY;
	if (keyStates['e'])
		zRot -= ROT_SENSITVITY;

	// calc the rot for the inner cube
	mat4 rot = glm::rotate(mat4(), glm::radians(-xRot), vec3(1.0, 0.0, 0.0));
	rot = glm::rotate(rot, glm::radians(-yRot), vec3(0.0, 1.0, 0.0));
	rot = glm::rotate(rot, glm::radians(-zRot), vec3(0.0, 0.0, 1.0));
	drawCube(vec3(1.0), rot, false);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawCube(vec3(0.5), rot, true);

	// calc the rot for the outer cube
	rot = glm::rotate(mat4(), glm::radians(xRot), vec3(1.0, 0.0, 0.0));
	rot = glm::rotate(rot, glm::radians(yRot), vec3(0.0, 1.0, 0.0));
	rot = glm::rotate(rot, glm::radians(zRot), vec3(0.0, 0.0, 1.0));
	drawCube(vec3(1.0), rot, false);
	glutSwapBuffers();
}

void init()
{
	// set init light pos
	lightX = 0.0;
	lightY = 0.0;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);					// enable blending

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, WIDTH, HEIGHT);
	
	proj = perspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 1000.0f);

	// load the programs
	try
	{
		solidCubeProg = OpenGLShaderLoader::getInstance().createProgram("Shaders\\SquareVertexShader.glsl", "Shaders\\passThroughFrgtShader.glsl");
		transparentCubeProg = OpenGLShaderLoader::getInstance().createProgram("Shaders\\TransparentSquareVertexShader.glsl", "Shaders\\LightingFrgtShader.glsl");
	}
	catch (std::exception& msg)
	{
		cout << msg.what();
		exit(-1);
	}
}

void renderTimer(int val)
{
	glutPostRedisplay();
	glutTimerFunc(FPS_DELAY, renderTimer, 0);
}

void keyPress(unsigned char key, int x, int y)
{
	// set key as pressed
	keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y)
{
	// set key as not pressed
	keyStates[key] = false;
}

void mousePosTracker(int x, int y)
{
	// translate screen to world cords
	lightX = ((float)x / (WIDTH/2-1)) - 1.0;
	lightY = 1.0 - ((float)y / (HEIGHT/2-1));
}

void drawCube(vec3 scale, const mat4& rot, bool solid)
{
	GLuint scaleUni;
	GLuint projecUni;
	GLuint viewUni;
	GLuint rotUni;
	GLuint lghtPosUni;
	GLuint viewPosUni;
	mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	static const mat4 viewMatrix = lookAt(vec3(0.0, 0.0, Z_POS), vec3(0.0), vec3(0.0, 1.0, 0.0));
	
	if (solid)
	{
		// set up the solid cube
		glUseProgram(solidCubeProg);
		rotUni = glGetUniformLocation(solidCubeProg, "rot");
		scaleUni = glGetUniformLocation(solidCubeProg, "scale");
		projecUni = glGetUniformLocation(solidCubeProg, "projection");
		viewUni = glGetUniformLocation(solidCubeProg, "viewMatrix");
		lghtPosUni = glGetUniformLocation(solidCubeProg, "lightPos");
		viewPosUni = glGetUniformLocation(solidCubeProg, "viewPos");
	}
	else
	{
		// set up the transparent cube
		glUseProgram(transparentCubeProg);
		rotUni = glGetUniformLocation(transparentCubeProg, "rot");
		scaleUni = glGetUniformLocation(transparentCubeProg, "scale");
		projecUni = glGetUniformLocation(transparentCubeProg, "projection");
		viewUni = glGetUniformLocation(transparentCubeProg, "viewMatrix");
		lghtPosUni = glGetUniformLocation(transparentCubeProg, "lightPos");
		viewPosUni = glGetUniformLocation(transparentCubeProg, "viewPos");
	}

	// set the common uniforms
	glUniformMatrix4fv(scaleUni, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
	glUniformMatrix4fv(projecUni, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(viewUni, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(rotUni, 1, GL_FALSE, glm::value_ptr(rot));
	glUniform3f(lghtPosUni, lightX, lightY, Z_POS);
	glUniform3f(viewPosUni, 0.0, 0.0, Z_POS);

	// draw
	glDrawArrays(GL_QUADS, 0, 24);
}