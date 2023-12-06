#pragma once

#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <classes/ElementBufferObject.hpp>
#include <classes/VertexBufferObject.hpp>

class VertexArrayObject {
public:
    VertexArrayObject(VertexBufferObject *VBO, ElementBufferObject *EBO);
	~VertexArrayObject();

    void AddVertexAttribute(u_int location, u_int size, GLenum type);

    void Bind();
    void Unbind();
	unsigned int GetVAO();
    VertexBufferObject *GetVBO();
    ElementBufferObject *GetEBO();
	size_t GetIndicesSize();
private:
    unsigned int VAO;
    VertexBufferObject *VBO;
    ElementBufferObject *EBO;
	std::size_t indicesSize;
    std::vector<glm::uvec2> locationList;
    int totalStride = 0;
	
};