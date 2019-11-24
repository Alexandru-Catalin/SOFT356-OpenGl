//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include "LoadShaders.h"
#include <glm/glm.hpp> //includes GLM
#include <glm/ext/matrix_transform.hpp> // GLM: translate, rotate
#include <glm/ext/matrix_clip_space.hpp> // GLM: perspective and ortho 
#include <glm/gtc/type_ptr.hpp> // GLM: access to the value_ptr
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include<fstream>
#include<conio.h>
#include<string>
#include <vector>
#include <sstream>  

using namespace std;

// to use this example you will need to download the header files for GLM put them into a folder which you will reference in
// properties -> VC++ Directories -> libraries

enum VAO_IDs { Triangles, Indices, Colours, Tex, NumVAOs = 1 };
enum Buffer_IDs { ArrayBuffer, NumBuffers = 4 };
enum Attrib_IDs { vPosition = 0, cPosition = 1, tPosition = 2 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];
GLuint texture1;

const GLuint  NumVertices = 36;

//----------------------------------------------------------------------------
//
// init
//
#define BUFFER_OFFSET(a) ((void*)(a))

// v is a vertex
// vt is the texture coordinate of one vertex
// vn is the normal of one vertex
// f is a face


void findFile(
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals
)
{

	//read the file
	string find;
	cout << "Enter file name: \n";

	cin >> find;
	ifstream findFile("media/" + find);

	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;

	if (findFile.is_open()) 
	{	
		while (getline(findFile, find)) 
		{		
			std::cout << find << std::endl;
			if(find[0] == 'v' & find[1] == ' ') 
			{
				glm::vec3 vertex;
				istringstream iss(find.substr(2));
				iss >> vertex.x; 
				iss >> vertex.y; 
				iss >> vertex.z;
				temp_vertices.push_back(vertex);
				cout << vertex.x;
			}
			else if(find[0] == 'v' & find[1] == 't')
			{
				glm::vec2 uv;
				istringstream iss(find.substr(2));
				iss >> uv.x;
				iss>> uv.y;
				temp_uvs.push_back(uv);
				cout << uv.x;
			}
			else if (find[0] == 'v' & find[1] == 'n')
			{
				glm::vec3 normal;
				istringstream iss(find.substr(2));
				iss >> normal.x;
				iss >> normal.y;
				iss>> normal.z;
				temp_normals.push_back(normal);
				cout << normal.x;
			}
			else if (find[0] == 'f' & find[1] == ' ')
			{
				std::string v1, v2, v3, v4, vt1, vt2, vt3, vt4, vn1, vn2, vn3, vn4;
				unsigned int vertexIndex[4], uvIndex[4], normalIndex[4];
				istringstream iss(find.substr(2));
				string set1, set2, set3, set4;

				std::getline(iss, set1, ' ');
				std::getline(iss, set2, ' ');
				std::getline(iss, set3, ' ');
				std::getline(iss, set4, ' ');
				
				istringstream iss1(set1), iss2(set2), iss3(set3), iss4(set4);
				//set1
				std::getline(iss1, v1, '/');
				std::getline(iss1, v2, '/');
				std::getline(iss1, v3, '/');
				//set2
				std::getline(iss2, v4, '/');
				std::getline(iss2, vt1, '/');
				std::getline(iss2, vt2, '/');
				//set3
				std::getline(iss3, vt3, '/');
				std::getline(iss3, vt4, '/');
				std::getline(iss3, vn1, '/');
				//set4
				std::getline(iss4, vn2, '/');
				std::getline(iss4, vn3, '/');
				std::getline(iss4, vn4, '/');

				vertexIndex[0] = atoi(v1.c_str());
				uvIndex[0] = atoi(v2.c_str());
				normalIndex[0] = atoi(v3.c_str());

				vertexIndex[1] = atoi(v4.c_str());
				uvIndex[1] = atoi(vt1.c_str());
				normalIndex[1] = atoi(vt2.c_str());

				vertexIndex[2] = atoi(vt3.c_str());
				uvIndex[2] = atoi(vt4.c_str());
				normalIndex[2] = atoi(vn1.c_str());

				vertexIndex[3] = atoi(vn2.c_str());
				uvIndex[3] = atoi(vn3.c_str());
				normalIndex[3] = atoi(vn4.c_str());

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				vertexIndices.push_back(vertexIndex[3]);

				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
				uvIndices.push_back(uvIndex[3]);
				
				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);
				normalIndices.push_back(normalIndex[3]);
				
				cout << vertexIndex[0] << uvIndex[0] << normalIndex[0] << endl;
			}	
		}
	}

	int count = 0;
	glm::vec3 vertexTemp;
	glm::vec2 uvTemp;
	glm::vec3 normalTemp;

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

		if (count == 0) 
		{
			//Save A
			vertexTemp = vertex;
			uvTemp = uv;
			normalTemp = normal;
		}
		else if(count == 2)	
		{
			//Push C
			out_vertices.push_back(vertex);
			out_uvs.push_back(uv);
			out_normals.push_back(normal);
		}
		else if(count == 3) 
		{
			//push A
			out_vertices.push_back(vertexTemp);
			out_uvs.push_back(uvTemp);
			out_normals.push_back(normalTemp);

			count = -1;
		}
		count++;
	}
	findFile.close();
}


void
init(vector<glm::vec3> vertices,
vector<glm::vec2> uvs,
vector<glm::vec3> normals)
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


	//GLfloat vertices[][3] = {
	//	{0.5f,  0.5f, -0.5f},  //0 top right
	//	{0.5f, -0.5f, -0.5f},  //1 bottom right
	//	{-0.5f, -0.5f, -0.5f}, //2 bottom left
	//	{-0.5f,  0.5f, -0.5f},  //3 top left

	//	{0.5f,  0.5f, 0.5f},  //4 top right
	//	{0.5f, -0.5f, 0.5f},  //5 bottom right
	//	{-0.5f, -0.5f, 0.5f}, //6 bottom left
	//	{-0.5f,  0.5f, 0.5f}  //7 top left 
	//};
	//GLuint indices[][3] = {  // note that we start from 0!
	//	{0, 3, 1},  // first Triangle front
	//	{3, 2, 1},   // second Triangle
	//	
	//	{4, 7, 0 },
	//	{7, 3, 0 },
	//	
	//	{1, 2, 5 },
	//	{2, 6, 5 },
	//	
	//	{5, 4, 0 },
	//	{0, 1, 5 },
	//	
	//	{2, 3, 7 },
	//	{7, 6, 2 },
	//	
	//	{4, 5, 7 },  // first Triangle back
	//	{7, 5, 6 }   // second Triangle
	//};

	//GLfloat  colours[][4] = {
	//	{ 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },  
	//	{ 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, 
	//	{ 0.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, 
	//};
	//GLfloat  texture_coords[] = {
	//	 1.0f, 1.0f,
	//	 1.0f, 0.0f,
	//	 0.0f, 0.0f,
	//	 0.0f, 1.0f,

	//	 0.0f, 1.0f,
	//     0.0f, 0.0f,
	//	 1.0f, 0.0f,
	//	 1.0f, 1.0f,

	//	
	//
	//};

	glGenBuffers(NumBuffers, Buffers);
	
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	

	glVertexAttribPointer(vPosition, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));
	
	//Colour Binding
	//glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	//glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), colours, 0);


	glVertexAttribPointer(cPosition, 4, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	//Texture Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Tex]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coords), texture_coords, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(tPosition, 2, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	// load and create a texture 
	// -------------------------
	
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	GLint width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load("media/textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	glUniform1i(glGetUniformLocation(program, "texture1"), 0);


	// creating the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -1.0f));

	// creating the view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

	// Adding all matrices up to create combined matrix
	glm::mat4 mvp = projection * view * model;


	//adding the Uniform to the shader
	int mvpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(mvp));

	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(cPosition); 
	glEnableVertexAttribArray(tPosition);
}


//----------------------------------------------------------------------------

 //display


void
display(void)
{
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT);
	// bind textures on corresponding texture units
	glFrontFace(GL_CW);
	/*glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);*/

	glBindVertexArray(VAOs[Triangles]);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glDrawElements(GL_TRIANGLES, NumVertices, GL_UNSIGNED_INT, 0);
	
}

//----------------------------------------------------------------------------
//
// main
//


int
main(int argc, char** argv)
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Textured Cube", NULL, NULL);

	glfwMakeContextCurrent(window);
	glewInit();

	
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;
	findFile(vertices, uvs, normals);
	init(vertices, uvs,	normals);
	while (!glfwWindowShouldClose(window))
	{
		// uncomment to draw only wireframe 
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		display();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();

}
