#include <classes/Game/CrossHair.hpp>

CrossHair::CrossHair(Shader *CrossHairShader)
{
	this->shader = CrossHairShader;

	vertices = {
		
		-0.001f, -0.05f, 
		-0.001f, 0.05f,
		0.001f, 0.05f,
		0.001f, -0.05f,
	
	
	-0.03f, -0.002f, 
		-0.03f, 0.002f,
		0.03f, 0.002f,
		0.03f, -0.002f,

		// -0.05f, 0.001f,
		// -0.05f, -0.001f,
		//  0.05f, -0.001f,
		//  0.05f, 0.001f,

		};

	shape = {
		0,1,2,
		2,3,0,
		4,5,6,
		6,7,4
		};

		t_vertexData *skyVertData = new t_vertexData();
		skyVertData->data = (u_char*)vertices.data();
		skyVertData->size = vertices.size() * sizeof(float);

		VAO = new VertexArrayObject(new VertexBufferObject(*skyVertData), new ElementBufferObject(shape), shader);
}

CrossHair::~CrossHair()
{}

void CrossHair::draw()
{
	// glDepthFunc(GL_ALWAYS); 

		VAO->Bind();
		
		glDrawElements(GL_TRIANGLES, vertices.size(), GL_UNSIGNED_INT, 0);
		VAO->Unbind();

	// glDepthFunc(GL_LESS); 
}