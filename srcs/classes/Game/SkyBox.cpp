#include <classes/Game/SkyBox.hpp>
# include <cmath>
# include <classes/Texture/TextureLoader.hpp>

float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

// float skyboxVertices[] = {
// 	// Front face
//         1.0f, 1.0f, 1.0f,
//         -1.0f, 1.0f, 1.0f,
//         -1.0f, -1.0f, 1.0f,
//         1.0f, -1.0f, 1.0f,

//         // Back face
//         1.0f, 1.0f, -1.0f,
//         -1.0f, 1.0f, -1.0f,
//         -1.0f, -1.0f, -1.0f,
//         1.0f, -1.0f, -1.0f,

//         // Top face
//         1.0f, 1.0f, 1.0f,
//         -1.0f, 1.0f, 1.0f,
//         -1.0f, 1.0f, -1.0f,
//         1.0f, 1.0f, -1.0f,

//         // Bottom face
//         1.0f, -1.0f, 1.0f,
//         -1.0f, -1.0f, 1.0f,
//         -1.0f, -1.0f, -1.0f,
//         1.0f, -1.0f, -1.0f,

//         // Left face
//         -1.0f, 1.0f, 1.0f,
//         -1.0f, -1.0f, 1.0f,
//         -1.0f, -1.0f, -1.0f,
//         -1.0f, 1.0f, -1.0f,

//         // Right face
//         1.0f, 1.0f, 1.0f,
//         1.0f, -1.0f, 1.0f,
//         1.0f, -1.0f, -1.0f,
//         1.0f, 1.0f, -1.0f
// 		};

// float skyboxTexCoords[] = { 
// 		0.0f, 0.0f,
// 		1.0f, 0.0f,
// 		1.0f, 1.0f,
// 		0.0f, 1.0f,
// 				0.0f, 0.0f,
// 		1.0f, 0.0f,
// 		1.0f, 1.0f,
// 		0.0f, 1.0f,
// 				0.0f, 0.0f,
// 		1.0f, 0.0f,
// 		1.0f, 1.0f,
// 		0.0f, 1.0f,
// 				0.0f, 0.0f,
// 		1.0f, 0.0f,
// 		1.0f, 1.0f,
// 		0.0f, 1.0f,
		
// 		};
float varTier = 0.333f;
float varTier2 = 0.666f;

float skyboxTexCoords[] = {
	// Front face
       	
        0.0f, varTier,
				0.0f, varTier2,
        0.25f, varTier2,
        0.25f, varTier,
		
        // Back face
		0.75f, varTier,
		0.75f, varTier2,
		0.5f,  varTier2,
		0.5f,  varTier,



        // Top face

		

        // rev Bottom face
        1.0f, varTier,//2
        1.0f, varTier2,//1
        0.75f,  varTier2,//0
        0.75f,  varTier,//3
		


		// rev Top face
        0.25f, varTier,
        0.25f, varTier2,
        0.5f, varTier2,
        0.5f, varTier,



        // Left face
       	0.25f, 1.0f,
        0.25f, varTier2,
        0.5f, varTier2,
        0.5f, 1.0f,

        // Right face
        
        0.25f, 0.0f,
       	0.25f, varTier,
				0.5f, varTier,
        0.5f, 0.0f
		
	};



unsigned int skyShape[] = {0, 1, 2, 2, 3, 0};


unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
} 

SkyBox::SkyBox(Shader *skyBoxShader)
{

	this->shader = skyBoxShader;

	
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	std::vector<std::string> faces
    {
        std::filesystem::path("textures/skyBox/right.png"),
        std::filesystem::path("textures/skyBox/left.png"),
        std::filesystem::path("textures/skyBox/top.png"),
        std::filesystem::path("textures/skyBox/bot.png"),
        std::filesystem::path("textures/skyBox/front.png"),
        std::filesystem::path("textures/skyBox/back.png")
    };

	cubemapTexture = loadCubemap(faces);

	// std::vector<std::string> faces;
	// faces.push_back("textures/skyBox.png");
	// faces.push_back("textures/skyBox.png");
	// faces.push_back("textures/skyBox.png");
	// faces.push_back("textures/skyBox.png");
	// faces.push_back("textures/skyBox.png");
	// faces.push_back("textures/skyBox.png");

	// for (int i = 0; i < 6; i++)
	// {
	// 	Texture tex = TextureLoader::LoadTexture(faces[i]);
	// 	texture.push_back(tex);
	// }

	// vertices.resize(6);
	// int coo = 0;
	// int text = 0;
	// for (int j = 0; j < 6; j++) {
	// 	for (int i = 0; i < 4; i++) {
	// 	vertices[j].push_back(skyboxVertices[coo]);
	// 	vertices[j].push_back(skyboxVertices[coo+1]);
	// 	vertices[j].push_back(skyboxVertices[coo+2]);

	// 	vertices[j].push_back(skyboxTexCoords[text]);
	// 	vertices[j].push_back(skyboxTexCoords[text+1]);
	// 	vertices[j].push_back(j);
	// 	coo += 3;
	// 	text += 2;
	// }
	// }

	

	// shape.resize(6);
	// for (u_int i = 0; i < 6; i++) {
	// 	for (int j = 0; j < 6; j++) {
	// 		shape[i].push_back(skyShape[j]);
	// 	}
	// }

	

	// VAO.resize(6);
	// for (int i = 0; i < 6; i++)
	// {
	// 	t_vertexData *skyVertData = new t_vertexData();
	// 	skyVertData->data = (u_char*)vertices[i].data();
	// 	skyVertData->size = vertices[i].size() * sizeof(float);

	// 	VAO[i] = new VertexArrayObject(new VertexBufferObject(*skyVertData), new ElementBufferObject(shape[i]), skyBoxShader);
	// }
}

void SkyBox::drawSkybox(glm::mat4 matrix, glm::vec3 pos)
{
	
	// glDepthMask(GL_FALSE);
	// shader->Use();
	
	// shader->SetInt("skybox", 0);
	// shader->Setmat4("matrix", matrix);

	// glBindVertexArray(skyboxVAO);
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	// glDrawArrays(GL_TRIANGLES, 0, 36);
	// glBindVertexArray(0);
	// glDepthMask(GL_TRUE);
		glDepthMask(GL_FALSE);  // change depth function so depth test passes when values are equal to depth buffer's content
		shader->Use();
		shader->SetInt("skybox", 0);
        shader->Setmat4("matrix", matrix);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthMask(GL_TRUE); 


	// 	VAO[0]->Bind();

	// if (Shader::GetActiveShader()) {
	// 	Shader::GetActiveShader()->Setmat4("matrix", matrix);
	// 	Shader::GetActiveShader()->SetFloat("playerPosX", pos.x);
	// 	Shader::GetActiveShader()->SetFloat("playerPosY", pos.y);
	// 	Shader::GetActiveShader()->SetFloat("playerPosZ", pos.z);
	// }
	
	// glDepthMask(GL_FALSE);

	// for (int i = 0; i < 6; i++)
	// {
	// 	VAO[i]->Bind();
	// 	glBindTexture(GL_TEXTURE_2D, texture[i].id);
	// 	// glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox->texture.id);
		
	// 	glDrawElements(GL_TRIANGLES, vertices[i].size(), GL_UNSIGNED_INT, 0);
	// 	VAO[i]->Unbind();
	// }

	// glDepthMask(GL_TRUE);
}

SkyBox::~SkyBox()
{
}

void SkyBox::setRenderDistance(unsigned int renderDistance)
{

}

void SkyBox::replace(int x, int y)
{
	
}