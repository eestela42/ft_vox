#include <classes/Game/SkyBox.hpp>
# include <cmath>
# include <classes/Texture/TextureLoader.hpp>

float skyboxVertices[] = {-1.0f,  1.0f, -1.0f,-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f,-1.0f,  1.0f, -1.0f,-1.0f, -1.0f,  1.0f,-1.0f, -1.0f, -1.0f,-1.0f,  1.0f, -1.0f,-1.0f,  1.0f, -1.0f,-1.0f,  1.0f,  1.0f,-1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f,-1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f,-1.0f, -1.0f,  1.0f,-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,-1.0f,  1.0f,  1.0f,-1.0f,  1.0f, -1.0f,-1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f};

float skyboxTexCoords[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 0.0f, 1.0f};







SkyBox::SkyBox(Shader *skyBoxShader)
{

	this->shader = skyBoxShader;

	std::vector<std::string> faces;
	faces.push_back("textures/sky.jpeg");
	faces.push_back("textures/sky.jpeg");
	faces.push_back("textures/sky.jpeg");
	faces.push_back("textures/sky.jpeg");
	faces.push_back("textures/sky.jpeg");
	faces.push_back("textures/sky.jpeg");

	for (int i = 0; i < 6; i++)
	{
		Texture tex = TextureLoader::LoadTexture(faces[i]);
		texture.push_back(tex);
	}

	vertices.resize(6);
	int pos = 0;
	for (int j = 0; j < 6; j++) {
		for (int i = 0; i < 6; i++) {
		vertices[j].push_back(skyboxVertices[pos]);
		vertices[j].push_back(skyboxVertices[pos+1]);
		vertices[j].push_back(skyboxVertices[pos+2]);
		vertices[j].push_back(skyboxTexCoords[i*2]);
		vertices[j].push_back(skyboxTexCoords[i*2+1]);
		pos += 3;
	}
	}

	

	shape.resize(6);
	for (u_int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			shape[i].push_back(j);
		}
	}

	

	VAO.resize(6);
	for (int i = 0; i < 6; i++)
	{
		t_vertexData *skyVertData = new t_vertexData();
		skyVertData->data = (u_char*)vertices[i].data();
		skyVertData->size = sizeof(skyboxVertices)/6;

		VAO[i] = new VertexArrayObject(new VertexBufferObject(*skyVertData), new ElementBufferObject(shape[i]), skyBoxShader);
	}
}

void SkyBox::drawSkybox(glm::mat4 matrix, glm::vec3 pos)
{
		VAO[0]->Bind();

	if (Shader::GetActiveShader()) {
		Shader::GetActiveShader()->Setmat4("matrix", matrix);
		Shader::GetActiveShader()->SetFloat("playerPosX", pos.x);
		Shader::GetActiveShader()->SetFloat("playerPosY", pos.y);
		Shader::GetActiveShader()->SetFloat("playerPosZ", pos.z);
	}
	
	glDepthMask(GL_FALSE);

	for (int i = 0; i < 6; i++)
	{
		VAO[i]->Bind();
		glBindTexture(GL_TEXTURE_2D, texture[i].id);
		// glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox->texture.id);
		
		glDrawElements(GL_TRIANGLES, vertices[i].size(), GL_UNSIGNED_INT, 0);
		VAO[i]->Unbind();
	}

	glDepthMask(GL_TRUE);
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