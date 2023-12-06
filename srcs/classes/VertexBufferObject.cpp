#include <classes/VertexBufferObject.hpp>

// Parameterized constructor
VertexBufferObject::VertexBufferObject(const std::vector<float>& vertices){
	verticesSize = vertices.size() * sizeof(vertices[0]);
    glGenBuffers(1, &index);
    glBindBuffer(GL_ARRAY_BUFFER, index);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices.data(), GL_STATIC_DRAW);
}

void VertexBufferObject::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, index);
}

void VertexBufferObject::DeleteBuffers() {
	glDeleteBuffers(1, &index);
}

int VertexBufferObject::GetSize() const {
    return verticesSize;
}