#include <classes/VAO/VertexArrayObject.hpp>

VertexArrayObject::VertexArrayObject(VertexBufferObject *VBO, ElementBufferObject *EBO, Shader *shader) : VBO(VBO), EBO(EBO), shader(shader) {
	if (EBO) {
		indicesSize = EBO->GetSize();
	}
	verticesSize = VBO->GetSize();

	glGenVertexArrays(1, &VAO);
	
    Bind();
    VBO->Bind();
	if (EBO) {
		EBO->Bind();
	}

	shader->Use();
	AddVertexAttributes(shader->GetVertexAttributes());
	Unbind();
}

void VertexArrayObject::AddVertexAttributes(std::vector<t_vertexAttribute> &vertexAttributes) {
	int totalStride = 0;

	for (auto const& x : vertexAttributes) {
		totalStride += x.size * 4;
	}
    int tempStride = 0;
    for (auto const& x : vertexAttributes) {
		if (x.type == GL_FLOAT) {
			glVertexAttribPointer(x.location, x.size, x.type, GL_FALSE, totalStride, (void *)(u_long)tempStride);
		}
		else if (x.type == GL_INT || x.type == GL_UNSIGNED_INT ) {
			glVertexAttribIPointer(x.location, x.size, x.type, totalStride, (void *)(u_long)tempStride);
		}
        
		glEnableVertexAttribArray(x.location);
        tempStride += x.size * 4;
    }
}

unsigned int VertexArrayObject::GetVAO() {
    return VAO;
}

VertexBufferObject *VertexArrayObject::GetVBO() {
    return VBO;
}

ElementBufferObject *VertexArrayObject::GetEBO() {
    return EBO;
}

size_t VertexArrayObject::GetIndicesSize() {
	return indicesSize;
}

size_t VertexArrayObject::GetVerticesSize() {
	return verticesSize;
}

void VertexArrayObject::Bind() {
	shader->Use();
    glBindVertexArray(VAO);
}

void VertexArrayObject::Unbind() {
    glBindVertexArray(0);
    for (std::vector<glm::uvec4>::iterator iterator = locationList.begin(); iterator != locationList.end(); iterator++) {
        glDisableVertexAttribArray(iterator->x);
    }
}

VertexArrayObject::~VertexArrayObject() {
    glDeleteVertexArrays(1, &VAO);
    VBO->DeleteBuffers();
	if (EBO) {
		EBO->DeleteBuffers();
		delete EBO;
	}
	delete VBO;
}