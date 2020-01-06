//////////////////////////////////////////////////////////////////////////////
//
//  SHaded Cube.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GLFW/glfw3.h"
#include "LoadShaders.h"
#include <glm/glm.hpp> //includes GML
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

enum VAO_IDs { Cube, NumVAOs = 1 };
enum Buffer_IDs { Triangles, Colours, Normals, Textures, Indices, NumBuffers = 5 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];
GLuint texture1;

GLuint shader;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


void readTexture(GLfloat& out_Ns,
	glm::vec4& out_Ka,
	glm::vec3& out_Kd,
	glm::vec3& out_Ks, float& trans, string& texture, string& file)
{

	string read;
	ifstream readFile("media/" + file + ".mtl");

	if (readFile.is_open()) {
		while (getline(readFile, read)) {

			if (read[0] == 'K' && read[1] == 'a')
			{
				istringstream iss(read.substr(2));
				iss >> out_Ka.x;
				iss >> out_Ka.y;
				iss >> out_Ka.z;
				out_Ka.w = 1.0;

			}
			else if (read[0] == 'K' && read[1] == 'd')
			{
				istringstream iss(read.substr(2));
				iss >> out_Kd.x;
				iss >> out_Kd.y;
				iss >> out_Kd.z;

			}
			else if (read[0] == 'K' && read[1] == 's')
			{
				istringstream iss(read.substr(2));
				iss >> out_Ks.x;
				iss >> out_Ks.y;
				iss >> out_Ks.z;

			}
			else if (read.substr(0, 6) == "map_Kd")
			{
				istringstream iss(read.substr(7));
				texture = iss.str();
			}
			else if (read[0] == 'd' && read[1] == ' ')
			{
				istringstream iss(read.substr(2));
				iss >> trans;

			}
			else if (read[0] == 'N' && read[1] == 's')
			{
				istringstream iss(read.substr(2));
				iss >> out_Ns;

			}

		}
		out_Ka.w = trans;
		texture = "media/" + texture;

	}
	readFile.close();
}

void findFile(
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals, string& file
)
{

	//read the file

	string find;
	ifstream findFile("media/" + file + ".obj");

	vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	vector<glm::vec3> temp_vertices;
	vector<glm::vec2> temp_uvs;
	vector<glm::vec3> temp_normals;

	if (findFile.is_open())
	{
		while (getline(findFile, find))
		{

			if (find[0] == 'v' && find[1] == ' ')
			{
				glm::vec3 vertex;
				istringstream iss(find.substr(2));
				iss >> vertex.x;
				iss >> vertex.y;
				iss >> vertex.z;
				temp_vertices.push_back(vertex);

			}
			else if (find[0] == 'v' && find[1] == 't')
			{
				glm::vec2 uv;
				istringstream iss(find.substr(2));
				iss >> uv.x;
				iss >> uv.y;
				temp_uvs.push_back(uv);


			}
			else if (find[0] == 'v' && find[1] == 'n')
			{
				glm::vec3 normal;
				istringstream iss(find.substr(2));
				iss >> normal.x;
				iss >> normal.y;
				iss >> normal.z;
				temp_normals.push_back(normal);

			}
			else if (find[0] == 'f' && find[1] == ' ')
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

				vertexIndex[0] = atoi(v1.c_str());
				uvIndex[0] = atoi(v2.c_str());
				normalIndex[0] = atoi(v3.c_str());

				vertexIndex[1] = atoi(v4.c_str());
				uvIndex[1] = atoi(vt1.c_str());
				normalIndex[1] = atoi(vt2.c_str());

				vertexIndex[2] = atoi(vt3.c_str());
				uvIndex[2] = atoi(vt4.c_str());
				normalIndex[2] = atoi(vn1.c_str());

				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);


				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);


				normalIndices.push_back(normalIndex[0]);
				normalIndices.push_back(normalIndex[1]);
				normalIndices.push_back(normalIndex[2]);

				bool check = false;
				if (set4 == "")
				{
					check = true;


				}
				else
				{

					check = false;

					//set4
					std::getline(iss4, vn2, '/');
					std::getline(iss4, vn3, '/');
					std::getline(iss4, vn4, '/');

					glm::vec3 vertexTemp;
					glm::vec2 uvTemp;
					glm::vec3 normalTemp;

					glm::vec3 vertex;
					glm::vec2 uv;
					glm::vec3 normal;

					glm::vec3 vertexTemp1;
					glm::vec2 uvTemp1;
					glm::vec3 normalTemp1;

					vertexIndices.push_back(vertexIndex[0]);
					uvIndices.push_back(uvIndex[0]);
					normalIndices.push_back(normalIndex[0]);
					vertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[2]);
					normalIndices.push_back(normalIndex[2]);
					vertexIndex[3] = atoi(vn2.c_str());
					uvIndex[3] = atoi(vn3.c_str());
					normalIndex[3] = atoi(vn4.c_str());
					vertexIndices.push_back(vertexIndex[3]);
					uvIndices.push_back(uvIndex[3]);
					normalIndices.push_back(normalIndex[3]);

				}

			}
		}
	}

	int count = 0;
	glm::vec3 vertexTemp;
	glm::vec2 uvTemp;
	glm::vec3 normalTemp;

	glm::vec3 vertexTemp1;
	glm::vec2 uvTemp1;
	glm::vec3 normalTemp1;
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}
	findFile.close();
}

//----------------------------------------------------------------------------
//
// init
//
#define BUFFER_OFFSET(a) ((void*)(a))



void
init(vector<glm::vec3> vertices,
	vector<glm::vec2> uvs,
	vector<glm::vec3> normals,
	string texture,
	glm::vec3 diffuse,
	glm::vec3 specular,
	GLfloat ns)
{
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Cube]);

	ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, "media/triangles.vert" },
		{ GL_FRAGMENT_SHADER, "media/triangles.frag" },
		{ GL_NONE, NULL }
	};

	shader = LoadShaders(shaders);
	glUseProgram(shader);

	//
	// configuring lighting
	//

	// ambient light
	glm::vec4 ambient = glm::vec4(1.0f);
	//adding the Uniform to the shader
	GLuint aLoc = glGetUniformLocation(shader, "ambient");
	glUniform4fv(aLoc, 1, glm::value_ptr(ambient));

	// light object
	glm::vec3 lightPos = glm::vec3(100.0f, 25.0f, 100.0f);
	GLuint dLightPosLoc = glGetUniformLocation(shader, "lightPos");
	glUniform3fv(dLightPosLoc, 1, glm::value_ptr(lightPos));


	// diffuse light
	glm::vec3 diffuseLight = diffuse;
	GLuint dLightLoc = glGetUniformLocation(shader, "dLight");
	glUniform3fv(dLightLoc, 1, glm::value_ptr(diffuseLight));

	// specular light
	glm::vec3 specularLight = specular;
	GLfloat shininess = ns; //128 is max value
	GLuint sLightLoc = glGetUniformLocation(shader, "sLight");
	GLuint sShineLoc = glGetUniformLocation(shader, "sShine");
	glUniform3fv(sLightLoc, 1, glm::value_ptr(specularLight));
	glUniform1fv(sShineLoc, 1, &shininess);


	
	vector<glm::vec4> colors;
	for (size_t i = 0; i < 36; i++)
	{
		colors.push_back(glm::vec4(1.0f));
	}
	
	GLfloat  colours[][4] = {
	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },

	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },

	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },

	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },

	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },

	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }

	};

	glGenBuffers(NumBuffers, Buffers);

	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Triangles]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[Indices]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/



	glVertexAttribPointer(Triangles, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	//Colour Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Colours]);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(colours), colours, 0);

	glVertexAttribPointer(Colours, 4, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));


	//Colour Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Normals]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);


	glVertexAttribPointer(Normals, 3, GL_FLOAT,
		GL_FALSE, 0, BUFFER_OFFSET(0));

	//Texture Binding
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[Textures]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(Textures, 2, GL_FLOAT,
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
	unsigned char* data = stbi_load(texture.c_str(), &width, &height, &nrChannels, 0);
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


	glUniform1i(glGetUniformLocation(shader, "texture1"), 0);


	// creating the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));


	// creating the view matrix
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3, 0.1f, 20.0f);

	// Adding all matrices up to create combined matrix
	glm::mat4 mv = view * model;


	//adding the Uniform to the shader
	int mvLoc = glGetUniformLocation(shader, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));
	//adding the Uniform to the shader
	int pLoc = glGetUniformLocation(shader, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));


	glEnableVertexAttribArray(Triangles);
	glEnableVertexAttribArray(Colours);
	glEnableVertexAttribArray(Textures);
	glEnableVertexAttribArray(Normals);
}


//----------------------------------------------------------------------------
//
// display
//

void
display(GLfloat delta)
{
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// per-frame time logic
		// --------------------
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glClearBufferfv(GL_COLOR, 0, black);
	glClear(GL_COLOR_BUFFER_BIT);

	// bind textures on corresponding texture units
	//glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	// creating the model matrix
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
	model = glm::rotate(model, glm::radians(delta), glm::vec3(1.0f, 0.0f, 0.0f));


	// creating the view matrix
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	// creating the projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	// Adding all matrices up to create combined matrix
	glm::mat4 mv = view * model;


	//adding the Uniform to the shader
	int mvLoc = glGetUniformLocation(shader, "mv_matrix");
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));
	//adding the Uniform to the shader
	int pLoc = glGetUniformLocation(shader, "p_matrix");
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(VAOs[Cube]);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	model = glm::translate(model, glm::vec3(1.0f));
	mv = view * model;
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glDrawArrays(GL_TRIANGLES, 0, 36);

}

//----------------------------------------------------------------------------
//
// main
//


int
main(int argc, char** argv)
{
	vector<glm::vec3> vertices;
	vector<glm::vec2> uvs;
	vector<glm::vec3> normals;

	string find;
	cin >> find;

	GLfloat Ns;
	glm::vec4 Ka;
	glm::vec3 Kd;
	glm::vec3 Ks;
	float trans;
	string texture;

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(800, 600, "Shaded Cube", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();


	findFile(vertices, uvs, normals, find);
	readTexture(Ns, Ka, Kd, Ks, trans, texture, find);
	init(vertices, uvs, normals, texture, Kd, Ks, Ns);


	GLfloat timer = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		// uncomment to draw only wireframe 
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		processInput(window);
		display(timer);
		glfwSwapBuffers(window);
		glfwPollEvents();
		timer += 0.1f;
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}