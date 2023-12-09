#pragma once

#include <vector>
#include <glad/glad.h>

class VertexBufferObject {
public:
    VertexBufferObject(const std::vector<int>& vertices);

	void Bind();
	void DeleteBuffers();
    int GetSize() const;
private:
    unsigned int index;
    int verticesSize;
};