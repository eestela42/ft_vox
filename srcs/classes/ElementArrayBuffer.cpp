#include <classes/ElementArrayBuffer.hpp>

// Parameterized constructor
ElementArrayBuffer::ElementArrayBuffer(const std::vector<unsigned int>& indices){
	indicesSize = indices.size() * sizeof(unsigned int);
    glGenBuffers(1, &index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices.data(), GL_STATIC_DRAW);
}

void ElementArrayBuffer::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
}

void ElementArrayBuffer::DeleteBuffers() {
	glDeleteBuffers(1, &index);
}

int ElementArrayBuffer::GetSize() const {
    return indicesSize;
}