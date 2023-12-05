#pragma once

#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <classes/ElementArrayBuffer.hpp>

class VertexArrayObject {
public:
    VertexArrayObject(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    VertexArrayObject(ElementArrayBuffer *EBO, const std::vector<float>& vertices);
    VertexArrayObject(unsigned int VBO, const std::vector<unsigned int>& indices);
    VertexArrayObject(unsigned int VBO, ElementArrayBuffer *EBO);
    ~VertexArrayObject();

    void AddVertexAttribute(u_int location, u_int size, GLenum type);

    void Bind();
    void Unbind();
	unsigned int GetVAO();
    unsigned int GetVBO();
    ElementArrayBuffer *GetEBO();
private:
    VertexArrayObject();
    unsigned int VAO;
    unsigned int VBO;
    ElementArrayBuffer *EBO;
	std::size_t indexesSize;
    std::vector<glm::uvec2> locationList;
    int totalStride = 0;

    void SetupVBO(const std::vector<float>& vertices);
};