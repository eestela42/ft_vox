#include <classes/VAO/VertexArrayObject.hpp>

VertexArrayObject::VertexArrayObject(VertexBufferObject *VBO, ElementBufferObject *EBO) {
	this->VBO = VBO;
    this->EBO = EBO;
	indicesSize = EBO->GetSize();

	glGenVertexArrays(1, &VAO);
	
    Bind();
    VBO->Bind();
    EBO->Bind();
}

void VertexArrayObject::AddVertexAttribute(u_int location, u_int size, GLenum type) {
    locationList.push_back(glm::uvec2(location, size));
    GLsizei offset = totalStride;
    totalStride += size * sizeof(type);
    
    int tempStride = 0;
    for (std::vector<glm::uvec2>::iterator iterator = locationList.begin(); iterator != locationList.end(); iterator++) {
        glVertexAttribPointer(iterator->x, iterator->y, type, GL_FALSE, totalStride, (void *)(u_long)tempStride);
        tempStride += iterator->y * 4;
    }
    glEnableVertexAttribArray(location);
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

void VertexArrayObject::Bind() {
    glBindVertexArray(VAO);
}

void VertexArrayObject::Unbind() {
    glBindVertexArray(0);
    for (std::vector<glm::uvec2>::iterator iterator = locationList.begin(); iterator != locationList.end(); iterator++) {
        glDisableVertexAttribArray(iterator->x);
    }
}

VertexArrayObject::~VertexArrayObject() {
    glDeleteVertexArrays(1, &VAO);
    VBO->DeleteBuffers();
    EBO->DeleteBuffers();
}