//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include "LoadShaders.h"
#include <glm.hpp> //includes GML
#include <ext/matrix_transform.hpp> // GLM: translate, rotate
#include <ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <gtc/type_ptr.hpp> // GLM: access to the value_ptr

// to use this example you will need to download the header files for GLM put them into a folder which you will reference in
// properties -> VC++ Directories -> libraries

enum VAO_IDs { Triangles, Colours, NumVAOs = 1 };
enum Buffer_IDs { ArrayBuffer, NumBuffers = 2 };
enum Attrib_IDs { vPosition = 0, cPosition = 1 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];

const GLuint  NumVertices = 6;

//----------------------------------------------------------------------------
//
// init
//
#define BUFFER_OFFSET(a) ((void*)(a))


void
init(void)
{
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "media/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
		{ GL_NONE, NULL }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);



	GLfloat  vertices[NumVertices][3] = {
		{ -0.90f, -0.90f, 0.0f}, {  0.85f, -0.90f, 0.0f }, { -0.90f,  0.85f, 0.0f },  // Triangle 1
		{  0.90f, -0.85f, 0.0f }, {  0.90f,  0.90f, 0.0f }, { -0.85f,  0.90f, 0.0f }   // Triangle 2
	};
	GLfloat  colours[NumVertices][4] = {
		{ 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f },  // Triangle 1
		{ 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }   // Triangle 2
	};

	glGenBuffers(NumBuffers, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), colours, 0);




	glVertexAttribPointer(cPosition, 4, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
	
	// creating the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::translate(model, glm::vec3(0.0f, -1.0f, -3.0f));
	
	// creating the view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, 1.0f));

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 4.0f/3, 0.1f, 10.0f);
	
	// Adding all matrices up to create combined matrix
	glm::mat4 mvp = projection* view * model;


	//adding the Uniform to the shader
	int mvpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(cPosition);
}


//----------------------------------------------------------------------------
//
// display
//

void
display(void)
{
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glClearBufferfv(GL_COLOR, 0, black);

	glBindVertexArray(VAOs[Triangles]);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

//----------------------------------------------------------------------------
//
// main
//


int
main(int argc, char** argv)
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Coloured Triangles", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewInit();

	init();

	while (!glfwWindowShouldClose(window))
	{
		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
