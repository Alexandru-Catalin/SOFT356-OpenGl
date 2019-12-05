
<h1> Model Loader </h1>

<h2> Functionality </h2>

I'm using Visual Studio Professional 2019, and OpenGL Version 4.0

<h3> How to use it </h3>

Step.1: Open Visual Studio and locate the filed named "SampleProjects.sln" (you may have to reinstall the package, for that type in the console "nupengl.core").

Step.1.2: In case the glm library is not working, in visual studio you will have to right click on "Project3" select "Properties", "VC++ Directories", "Include Directories", "Edit", and locate the "glm" and "doc" folder. (Example: C:\Users\amarcu\Desktop\Model Loader\SOFT356-OpenGl-master\SOFT356-OpenGl-master\SampleCode-master).

Step.2: Press the green button "Local Windows Debugger".

Step.3: Type the name of the ".obj" file that you want to open (Example: Creeper).

<h4> How does it work </h4>

Everything it's happening inside the the "TexturedCube.cpp". Inside it I'm reading the object and his texture and display it. 

<h5>This are the main categories inside the script:</h5>

<h6><b><i>void readTexture()</i></b></h6>

Here I'm reading the "Creeper.mtl" file.

To be able to open the file, I'm setting up the path for it:

<p>string read;</p>
	<p>ifstream readFile("media/" + file + ".mtl");</p>
  
Everything it's located inside the "media" folder, and from there we set it to look only for the ".mtl" format. In this way it will allow us to type only the name of the file(In our case "Creeper") and the program will automatcly read the the ".mtl" file.

The ".mtl" file contain different types of vectors witch we need to separate and save them separately. For that I'm using an else if statement.
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

		}
		out_Ka.w = trans;
		texture = "media/" + texture;

	}
	readFile.close();
  
  <h7><b><i>void findFile()</i></b></h7>
  
  The method to read the ".obj" file it's similar to reading the texture for it, with the small difference that we have to make a split in order to be able to read and save the faces.
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

				
			}
		}
	}
  
  Because each face of the "Creeper" contained 4 points, and OpenGL can't read quad's, we need to triangulate each face in order to be able to load the cube correctly.
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

		// Put the attributes in buffers
		if (count != 3) {
			out_vertices.push_back(vertex);
			out_uvs.push_back(uv);
			out_normals.push_back(normal);
		}

		if (count == 0)
		{
			//Save A
			vertexTemp = vertex;
			uvTemp = uv;
			normalTemp = normal;
		}
		else if (count == 2)
		{
			//Push C

			vertexTemp1 = vertex;
			uvTemp1 = uv;
			normalTemp1 = normal;

		}
		else if (count == 3)
		{
			//push A


			

			out_vertices.push_back(vertexTemp);
			out_uvs.push_back(uvTemp);
			out_normals.push_back(normalTemp);

			out_vertices.push_back(vertexTemp1);
			out_uvs.push_back(uvTemp1);
			out_normals.push_back(normalTemp1);

			out_vertices.push_back(vertex);
			out_uvs.push_back(uv);
			out_normals.push_back(normal);


			count = -1;
		}
		count++;
    }
	findFile.close();}
  
