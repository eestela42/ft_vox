#include <classes/Game/SkyBox.hpp>
# include <cmath>

float skyboxVertices[] = {-1.0f,  1.0f, -1.0f,-1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f,-1.0f,  1.0f, -1.0f,-1.0f, -1.0f,  1.0f,-1.0f, -1.0f, -1.0f,-1.0f,  1.0f, -1.0f,-1.0f,  1.0f, -1.0f,-1.0f,  1.0f,  1.0f,-1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f, 1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f,-1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f, -1.0f,  1.0f,-1.0f, -1.0f,  1.0f,-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,-1.0f,  1.0f,  1.0f,-1.0f,  1.0f, -1.0f,-1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,-1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f};

SkyBox::SkyBox(Shader *skyBoxShader, Texture skyBoxTexture)
{
	this->shader = skyBoxShader;
	this->texture = skyBoxTexture;
	for (u_int i = 0; i < 108; i++) {
		vertices.push_back(skyboxVertices[i]);
	}
	for (u_int i = 0; i < 36; i++) {
		shape.push_back(i);
	}


	t_vertexData *skyVertData = new t_vertexData();
	skyVertData->data = (u_char*)vertices.data();
	skyVertData->size = sizeof(skyboxVertices);
	VAO = new VertexArrayObject(new VertexBufferObject(*skyVertData), new ElementBufferObject(shape), skyBoxShader);


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