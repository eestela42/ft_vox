#include <classes/ElementBufferObject.hpp>
#include <iostream>

// Parameterized constructor
ElementBufferObject::ElementBufferObject(const std::vector<unsigned int>& indices){
	std::cout << "indices size : " << indices.size() << std::endl;
	indicesSize = indices.size() * sizeof(indices[0]);
    glGenBuffers(1, &index);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices.data(), GL_STATIC_DRAW);
}

void ElementBufferObject::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
}

void ElementBufferObject::DeleteBuffers() {
	glDeleteBuffers(1, &index);
}

int ElementBufferObject::GetSize() const {
    return indicesSize;
}