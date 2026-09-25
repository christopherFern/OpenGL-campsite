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
	GLint vPosition;
	GLint vNormal;
	int vs;
	int fs;

	glGenVertexArrays(1, &triangleVAO);
	glBindVertexArray(triangleVAO);

	GLfloat vertices[16][4] = {
		//Bottom ring of the cylinder plane for the tree tunk
		{cos(0 * PI / 180), 0.0, sin(0 * PI / 180), 1.0}, //0
		{cos(45 * PI / 180), 0.0, sin(45 * PI / 180), 1.0}, //1
		{cos(90 * PI / 180), 0.0, sin(90 * PI / 180), 1.0}, //2
		{cos(135 * PI / 180), 0.0, sin(135 * PI / 180), 1.0}, //3
		{cos(180 * PI / 180), 0.0, sin(180 * PI / 180), 1.0}, //4
		{cos(225 * PI / 180), 0.0, sin(225 * PI / 180), 1.0}, //5
		{cos(270 * PI / 180), 0.0, sin(270 * PI / 180), 1.0}, //6
		{cos(315 * PI / 180), 0.0, sin(315 * PI / 180), 1.0}, //7

		//Top ring of the cylinder plane for the tree trunk
		{cos(0 * PI / 180), 5.0, sin(0 * PI / 180), 1.0}, //8
		{cos(45 * PI / 180), 5.0, sin(45 * PI / 180), 1.0}, //9
		{cos(90 * PI / 180), 5.0, sin(90 * PI / 180), 1.0}, //10
		{cos(135 * PI / 180), 5.0, sin(135 * PI / 180), 1.0}, //11
		{cos(180 * PI / 180), 5.0, sin(180 * PI / 180), 1.0}, //12
		{cos(225 * PI / 180), 5.0, sin(225 * PI / 180), 1.0}, //13
		{cos(270 * PI / 180), 5.0, sin(270 * PI / 180), 1.0}, //14
		{cos(315 * PI / 180), 5.0, sin(315 * PI / 180), 1.0} //15
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
	//Leaves for the tree
	GLfloat leafVertices[9][4] = {
		{4*cos(0 * PI / 180), 5.0, 4*sin(0 * PI / 180), 1.0}, //0
		{4*cos(45 * PI / 180), 5.0, 4*sin(45 * PI / 180), 1.0}, //1
		{4*cos(90 * PI / 180), 5.0, 4*sin(90 * PI / 180), 1.0}, //2
		{4*cos(135 * PI / 180), 5.0, 4*sin(135 * PI / 180), 1.0}, //3
		{4*cos(180 * PI / 180), 5.0, 4*sin(180 * PI / 180), 1.0}, //4
		{4*cos(225 * PI / 180), 5.0, 4*sin(225 * PI / 180), 1.0}, //5
		{4*cos(270 * PI / 180), 5.0, 4*sin(270 * PI / 180), 1.0}, //6
		{4*cos(315 * PI / 180), 5.0, 4*sin(315 * PI / 180), 1.0}, //7
		{0.0, 15.0, 0.0, 1.0} //8
	};
	GLfloat leafNormals[9][3] = {
		{cos(0 * PI / 180), 0.0, sin(0 * PI / 180)}, //0
		{cos(45 * PI / 180), 0.0, sin(45 * PI / 180)}, //1
		{cos(90 * PI / 180), 0.0, sin(90 * PI / 180)}, //2
		{cos(135 * PI / 180), 0.0, sin(135 * PI / 180)}, //3
		{cos(180 * PI / 180), 0.0, sin(180 * PI / 180)}, //4
		{cos(225 * PI / 180), 0.0, sin(225 * PI / 180)}, //5
		{cos(270 * PI / 180), 0.0, sin(270 * PI / 180)}, //6
		{cos(315 * PI / 180), 0.0, sin(315 * PI / 180)}, //7
		{0.0, -1.0, 0.0} //8
	};
	GLushort leafIndexes[24] = {
		0, 1, 8,
		1, 2, 8,
		2, 3, 8,
		3, 4, 8,
		4, 5, 8,
		5, 6, 8,
		6, 7, 8,
		7, 0, 8
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
	int modelLoc;
	int normalLoc;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	model = glm::rotate(glm::mat4(1.0), angle, glm::vec3(0.0, 1.0, 0.0));

	view = glm::lookAt(glm::vec3(eyex, eyey, eyez),
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat3 normal = glm::transpose(glm::inverse(glm::mat3(view*model)));
					
	modelViewPerspective = projection * view * model;

	glUseProgram(program);
	modelLoc = glGetUniformLocation(program,"model");
	glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(modelViewPerspective));
	normalLoc = glGetUniformLocation(program,"normalMat");
	glUniformMatrix3fv(normalLoc, 1, 0, glm::value_ptr(normal));

	glBindVertexArray(triangleVAO);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_SHORT, NULL);

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
	eyez = 10.0;
	eyey = 15.0;

	theta = 0.0;
	phi = -1.5;
	r = 4.0;

	glfwSwapInterval(1);

	// GLFW main loop, display model, swapbuffer and check for input

	while (!glfwWindowShouldClose(window)) {
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

}
