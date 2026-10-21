/************************************************
 *
 *             Example Four
 *
 *  A basic OpenGL program that draws a
 *  triangle on the screen in perspective with
 *  simple control over the eye position.  
 *  This program illustrates the construction of
 *  perspective and viewing transformations.
 *
 ************************************************/

#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/glew.h>
#define GLFW_DLL
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shaders.h"
#include <stdio.h>

GLuint program;			// shader programs
GLuint triangleVAO;		// the data to be displayed
GLuint coneVAO;
GLuint planeVAO;
float angle = 0.0;
double theta, phi;		// user's position  on a sphere centered on the object
double r;				// radius of the sphere
GLuint ibuffer;

glm::mat4 projection;	// projection matrix
float eyex, eyey, eyez;	// eye position
const float PI = 3.14159265358979323846;

/*
 *  The init procedure creates the OpenGL data structures
 *  that contain the triangle geometry, compiles our
 *  shader program and links the shader programs to
 *  the data.
 */

void init() {
	GLuint vbuffer;
	GLuint coneVbuffer;
	GLuint planeVbuffer;
	GLint vPosition;
	GLint vNormal;
	int vs;
	int fs;

	glGenVertexArrays(1, &triangleVAO);
	glBindVertexArray(triangleVAO);

	GLfloat vertices[16][4] = {
		//Bottom ring of the cylinder plane for the tree tunk
		{0.65 * cos(0 * PI / 180), 0.0, 0.65 * sin(0 * PI / 180), 1.0}, //0
		{0.65 * cos(45 * PI / 180), 0.0, 0.65 * sin(45 * PI / 180), 1.0}, //1
		{0.65 * cos(90 * PI / 180), 0.0, 0.65 * sin(90 * PI / 180), 1.0}, //2
		{0.65 * cos(135 * PI / 180), 0.0, 0.65 * sin(135 * PI / 180), 1.0}, //3
		{0.65 * cos(180 * PI / 180), 0.0, 0.65 * sin(180 * PI / 180), 1.0}, //4
		{0.65 * cos(225 * PI / 180), 0.0, 0.65 * sin(225 * PI / 180), 1.0}, //5
		{0.65 * cos(270 * PI / 180), 0.0, 0.65 * sin(270 * PI / 180), 1.0}, //6
		{0.65 * cos(315 * PI / 180), 0.0, 0.65 * sin(315 * PI / 180), 1.0}, //7

		//Top ring of the cylinder plane for the tree trunk
		{0.65 * cos(0 * PI / 180), 2.5, 0.65 * sin(0 * PI / 180), 1.0}, //8
		{0.65 * cos(45 * PI / 180), 2.5, 0.65 * sin(45 * PI / 180), 1.0}, //9
		{0.65 * cos(90 * PI / 180), 2.5, 0.65 * sin(90 * PI / 180), 1.0}, //10
		{0.65 * cos(135 * PI / 180), 2.5, 0.65 * sin(135 * PI / 180), 1.0}, //11
		{0.65 * cos(180 * PI / 180), 2.5, 0.65 * sin(180 * PI / 180), 1.0}, //12
		{0.65 * cos(225 * PI / 180), 2.5, 0.65 * sin(225 * PI / 180), 1.0}, //13
		{0.65 * cos(270 * PI / 180), 2.5, 0.65 * sin(270 * PI / 180), 1.0}, //14
		{0.65 * cos(315 * PI / 180), 2.5, 0.65 * sin(315 * PI / 180), 1.0} //15
	};
	GLfloat normals[16][3] = {
		{cos(0 * PI / 180), 0.0, sin(0 * PI / 180)}, //0
		{cos(45 * PI / 180), 0.0, sin(45 * PI / 180)}, //1
		{cos(90 * PI / 180), 0.0, sin(90 * PI / 180)}, //2
		{cos(135 * PI / 180), 0.0, sin(135 * PI / 180)}, //3
		{cos(180 * PI / 180), 0.0, sin(180 * PI / 180)}, //4
		{cos(225 * PI / 180), 0.0, sin(225 * PI / 180)}, //5
		{cos(270 * PI / 180), 0.0, sin(270 * PI / 180)}, //6
		{cos(315 * PI / 180), 0.0, sin(315 * PI / 180)}, //7

		{cos(0 * PI / 180), 0.0, sin(0 * PI / 180)}, //8
		{cos(45 * PI / 180), 0.0, sin(45 * PI / 180)}, //9
		{cos(90 * PI / 180), 0.0, sin(90 * PI / 180)}, //10
		{cos(135 * PI / 180), 0.0, sin(135 * PI / 180)}, //11
		{cos(180 * PI / 180), 0.0, sin(180 * PI / 180)}, //12
		{cos(225 * PI / 180), 0.0, sin(225 * PI / 180)}, //13
		{cos(270 * PI / 180), 0.0, sin(270 * PI / 180)}, //14
		{cos(315 * PI / 180), 0.0, sin(315 * PI / 180)} //15
	};
	GLushort indexes[48] = { 
		0, 1, 8,
		8, 9, 1,
		1, 2, 9,
		9, 10, 2,
		2, 3, 10,
		10, 11, 3,
		3, 4, 11,
		11, 12, 4,
		4, 5, 12,
		12, 13, 5,
		5, 6, 13,
		13, 14, 6,
		6, 7, 14,
		14, 15, 7,
		7, 0, 15,
		15, 8, 0
	};

	/*
	 *  load the vertex coordinate data
	 */
	glGenBuffers(1, &vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices)+sizeof(normals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);
	
	/*
	 *  load the vertex indexes
	 */
	glGenBuffers(1, &ibuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	/*
	 *  compile and build the shader program
	 */
	vs = buildShader(GL_VERTEX_SHADER, (char*)"example4.vs");
	fs = buildShader(GL_FRAGMENT_SHADER, (char*)"example4.fs");
	program = buildProgram(vs,fs,0);

	/*
	 *  link the vertex coordinates to the vPosition
	 *  variable in the vertex program
	 */
	glUseProgram(program);
	vPosition = glGetAttribLocation(program,"vPosition");
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);
	vNormal = glGetAttribLocation(program, "vNormal");
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*) sizeof(vertices));
	glEnableVertexAttribArray(vNormal);

	GLfloat coneVertices[24][4];
	GLfloat coneNormals[24][3];
	for (int side = 0; side < 8; ++side) {
		float angle0 = side * 2.0f * PI / 8.0f;
		float angle1 = (side + 1) * 2.0f * PI / 8.0f;
		float middleAngle = (angle0 + angle1) * 0.5f;
		float normalLength = sqrt(2.0f);
		float nx = cos(middleAngle) / normalLength;
		float ny = 1.0f / normalLength;
		float nz = sin(middleAngle) / normalLength;
		int firstVertex = side * 3;

		coneVertices[firstVertex][0] = cos(angle0);
		coneVertices[firstVertex][1] = 0.0f;
		coneVertices[firstVertex][2] = sin(angle0);
		coneVertices[firstVertex][3] = 1.0f;
		coneVertices[firstVertex + 1][0] = cos(angle1);
		coneVertices[firstVertex + 1][1] = 0.0f;
		coneVertices[firstVertex + 1][2] = sin(angle1);
		coneVertices[firstVertex + 1][3] = 1.0f;
		coneVertices[firstVertex + 2][0] = 0.0f;
		coneVertices[firstVertex + 2][1] = 1.0f;
		coneVertices[firstVertex + 2][2] = 0.0f;
		coneVertices[firstVertex + 2][3] = 1.0f;

		for (int vertex = 0; vertex < 3; ++vertex) {
			coneNormals[firstVertex + vertex][0] = nx;
			coneNormals[firstVertex + vertex][1] = ny;
			coneNormals[firstVertex + vertex][2] = nz;
		}
	}

	glGenVertexArrays(1, &coneVAO);
	glBindVertexArray(coneVAO);
	glGenBuffers(1, &coneVbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, coneVbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coneVertices) + sizeof(coneNormals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(coneVertices), coneVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(coneVertices), sizeof(coneNormals), coneNormals);

	glUseProgram(program);
	vPosition = glGetAttribLocation(program, "vPosition");
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);
	vNormal = glGetAttribLocation(program, "vNormal");
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*) sizeof(coneVertices));
	glEnableVertexAttribArray(vNormal);

	GLfloat planeVertices[6][4] = {
		{ -50.0f, -0.02f, -50.0f, 1.0f },
		{  50.0f, -0.02f,  50.0f, 1.0f },
		{  50.0f, -0.02f, -50.0f, 1.0f },
		{ -50.0f, -0.02f, -50.0f, 1.0f },
		{ -50.0f, -0.02f,  50.0f, 1.0f },
		{  50.0f, -0.02f,  50.0f, 1.0f }
	};
	GLfloat planeNormals[6][3] = {
		{ 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }
	};

	glGenVertexArrays(1, &planeVAO);
	glBindVertexArray(planeVAO);
	glGenBuffers(1, &planeVbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, planeVbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices) + sizeof(planeNormals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(planeVertices), planeVertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(planeVertices), sizeof(planeNormals), planeNormals);

	glUseProgram(program);
	vPosition = glGetAttribLocation(program, "vPosition");
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);
	vNormal = glGetAttribLocation(program, "vNormal");
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, (void*) sizeof(planeVertices));
	glEnableVertexAttribArray(vNormal);

}

void framebufferSizeCallback(GLFWwindow *window, int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).

	if (h == 0)
		h = 1;

	float ratio = 1.0f * w / h;

	glfwMakeContextCurrent(window);

	glViewport(0, 0, w, h);

	projection = glm::perspective(0.7f, ratio, 1.0f, 100.0f);

}

/*
 *  This procedure is called each time the screen needs
 *  to be redisplayed
 */
void display() {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 modelViewPerspective;
	GLint modelLoc;
	GLint normalLoc;
	GLint colourLoc;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	model = glm::rotate(glm::mat4(1.0), angle, glm::vec3(0.0, 1.0, 0.0));

	view = glm::lookAt(glm::vec3(eyex, eyey, eyez),
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 1.0f, 0.0f));

	glUseProgram(program);
	modelLoc = glGetUniformLocation(program, "model");
	normalLoc = glGetUniformLocation(program, "normalMat");
	colourLoc = glGetUniformLocation(program, "objectColour");

	glm::mat3 normal = glm::transpose(glm::inverse(glm::mat3(view * model)));
	modelViewPerspective = projection * view * model;
	glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(modelViewPerspective));
	glUniformMatrix3fv(normalLoc, 1, 0, glm::value_ptr(normal));
	glBindVertexArray(planeVAO);
	glUniform4f(colourLoc, 0.025f, 0.16f, 0.045f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	const float coneRadii[4] = { 3.0f, 2.35f, 1.7f, 1.1f };
	const float coneBases[4] = { 2.5f, 4.25f, 6.0f, 7.75f };
	const float coneHeights[4] = { 2.4f, 2.3f, 2.2f, 2.0f };
	const glm::vec3 treePositions[] = {
		glm::vec3(-38.0f, 0.0f, -36.0f), glm::vec3(-23.0f, 0.0f, -40.0f),
		glm::vec3(-7.0f, 0.0f, -38.0f), glm::vec3(11.0f, 0.0f, -40.0f),
		glm::vec3(29.0f, 0.0f, -37.0f), glm::vec3(42.0f, 0.0f, -31.0f),
		glm::vec3(-42.0f, 0.0f, -19.0f), glm::vec3(-29.0f, 0.0f, -23.0f),
		glm::vec3(-14.0f, 0.0f, -25.0f), glm::vec3(17.0f, 0.0f, -25.0f),
		glm::vec3(32.0f, 0.0f, -21.0f), glm::vec3(44.0f, 0.0f, -13.0f),
		glm::vec3(-43.0f, 0.0f, 1.0f), glm::vec3(-29.0f, 0.0f, 5.0f),
		glm::vec3(-17.0f, 0.0f, 13.0f), glm::vec3(18.0f, 0.0f, 12.0f),
		glm::vec3(31.0f, 0.0f, 5.0f), glm::vec3(43.0f, 0.0f, 3.0f),
		glm::vec3(-39.0f, 0.0f, 21.0f), glm::vec3(-24.0f, 0.0f, 24.0f),
		glm::vec3(-9.0f, 0.0f, 29.0f), glm::vec3(10.0f, 0.0f, 27.0f),
		glm::vec3(28.0f, 0.0f, 23.0f), glm::vec3(41.0f, 0.0f, 19.0f),
		glm::vec3(-31.0f, 0.0f, 39.0f), glm::vec3(-12.0f, 0.0f, 42.0f),
		glm::vec3(9.0f, 0.0f, 41.0f), glm::vec3(30.0f, 0.0f, 40.0f),
		glm::vec3(-35.0f, 0.0f, -29.0f), glm::vec3(-17.0f, 0.0f, -32.0f),
		glm::vec3(0.0f, 0.0f, -29.0f), glm::vec3(18.0f, 0.0f, -33.0f),
		glm::vec3(38.0f, 0.0f, -25.0f), glm::vec3(-36.0f, 0.0f, -10.0f),
		glm::vec3(-22.0f, 0.0f, -14.0f), glm::vec3(-11.0f, 0.0f, -16.0f),
		glm::vec3(13.0f, 0.0f, -16.0f), glm::vec3(24.0f, 0.0f, -13.0f),
		glm::vec3(38.0f, 0.0f, -8.0f), glm::vec3(-38.0f, 0.0f, 10.0f),
		glm::vec3(-27.0f, 0.0f, 16.0f), glm::vec3(-13.0f, 0.0f, 22.0f),
		glm::vec3(14.0f, 0.0f, 21.0f), glm::vec3(25.0f, 0.0f, 14.0f),
		glm::vec3(38.0f, 0.0f, 13.0f), glm::vec3(-34.0f, 0.0f, 30.0f),
		glm::vec3(-19.0f, 0.0f, 34.0f), glm::vec3(0.0f, 0.0f, 36.0f),
		glm::vec3(18.0f, 0.0f, 33.0f), glm::vec3(38.0f, 0.0f, 31.0f)
	};
	const float treeScales[] = {
		0.9f, 1.0f, 0.8f, 1.05f, 0.9f, 1.0f,
		1.0f, 0.85f, 1.0f, 0.9f, 1.05f, 0.8f,
		0.9f, 1.0f, 0.8f, 1.0f, 0.9f, 1.05f,
		1.0f, 0.85f, 1.0f, 0.9f, 1.05f, 0.8f,
		0.9f, 1.0f, 0.85f, 1.0f,
		0.8f, 0.95f, 1.05f, 0.85f, 1.0f, 0.9f,
		0.8f, 1.0f, 0.9f, 1.05f, 0.85f, 1.0f,
		0.9f, 1.05f, 0.8f, 0.95f, 1.0f, 0.85f,
		1.05f, 0.9f, 0.8f, 1.0f
	};

	for (int tree = 0; tree < sizeof(treePositions) / sizeof(treePositions[0]); ++tree) {
		glm::mat4 treeModel = glm::translate(model, treePositions[tree]);
		treeModel = glm::scale(treeModel, glm::vec3(treeScales[tree]));
		glm::mat3 treeNormal = glm::transpose(glm::inverse(glm::mat3(view * treeModel)));
		glm::mat4 treeMVP = projection * view * treeModel;

		glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(treeMVP));
		glUniformMatrix3fv(normalLoc, 1, 0, glm::value_ptr(treeNormal));
		glUniform4f(colourLoc, 0.36f, 0.18f, 0.05f, 1.0f);
		glBindVertexArray(triangleVAO);
		glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_SHORT, NULL);

		glBindVertexArray(coneVAO);
		for (int i = 0; i < 4; ++i) {
			if (i == 1 || i == 3)
				glUniform4f(colourLoc, 0.08f, 0.42f, 0.12f, 1.0f);
			else
				glUniform4f(colourLoc, 0.28f, 0.62f, 0.20f, 1.0f);

			glm::mat4 coneModel = glm::translate(treeModel, glm::vec3(0.0f, coneBases[i], 0.0f));
			coneModel = glm::scale(coneModel, glm::vec3(coneRadii[i], coneHeights[i], coneRadii[i]));
			glm::mat3 coneNormal = glm::transpose(glm::inverse(glm::mat3(view * coneModel)));
			glm::mat4 coneMVP = projection * view * coneModel;
			glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(coneMVP));
			glUniformMatrix3fv(normalLoc, 1, 0, glm::value_ptr(coneNormal));
			glDrawArrays(GL_TRIANGLES, 0, 24);
		}
	}

}


/*
 *  Called each time a key is pressed on
 *  the keyboard.
 */

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		phi -= 0.1;
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		phi += 0.1;
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		theta += 0.1;
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		theta -= 0.1;
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		r = glm::max(2.0, r - 1.0);
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		r = glm::min(99.0, r + 1.0);

	 eyex = (float)(r*sin(theta)*cos(phi));
	 eyey = (float)(r*sin(theta)*sin(phi));
	 eyez = (float)(r*cos(theta));

}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}


int main(int argc, char **argv) {
	GLFWwindow *window;
	eyex = 10.0;
	eyey = 5.0;
	eyez = 10.0;
	theta = 1.5;
	phi = 1.5;
	r = 10.0;	

	// start by setting error callback in case something goes wrong

	glfwSetErrorCallback(error_callback);

	// initialize glfw

	if (!glfwInit()) {
		fprintf(stderr, "can't initialize GLFW\n");
	}

	// create the window used by our application

	window = glfwCreateWindow(512, 512, "Example Four", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// establish framebuffer size change and input callbacks

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glfwSetKeyCallback(window, key_callback);

	/*
	 *  initialize glew
	 */
	glfwMakeContextCurrent(window);
	GLenum error = glewInit();
	if(error != GLEW_OK) {
		printf("Error starting GLEW: %s\n",glewGetErrorString(error));
		exit(0);
	}

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glViewport(0, 0, 512, 512);

	projection = glm::perspective(0.7f, 1.0f, 1.0f, 100.0f);

	init();

	eyex = 0.0;
	eyey = 8.0;
	eyez = 22.0;

	theta = 0.35;
	phi = PI / 2.0;
	r = 23.4;

	glfwSwapInterval(1);

	// GLFW main loop, display model, swapbuffer and check for input

	while (!glfwWindowShouldClose(window)) {
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

}
