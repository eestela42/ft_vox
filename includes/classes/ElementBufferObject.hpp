#pragma once

#include <vector>
#include <glad/glad.h>

class ElementBufferObject {
public:
    ElementBufferObject(const std::vector<unsigned int>& indices);

	void Bind();
	void DeleteBuffers();
    int GetSize() const;
private:
    unsigned int index;
    int indicesSize;
};