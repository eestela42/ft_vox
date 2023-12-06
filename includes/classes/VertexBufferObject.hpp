#pragma once

#include <vector>
#include <glad/glad.h>

class VertexBufferObject {
public:
    VertexBufferObject(const std::vector<float>& vertices);

	void Bind();
	void DeleteBuffers();
    int GetSize() const;
private:
    unsigned int index;
    int verticesSize;
};