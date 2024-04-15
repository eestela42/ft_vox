#pragma once

#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <classes/ElementBufferObject.hpp>
#include <classes/VertexBufferObject.hpp>
#include <classes/Shader.hpp>

class VertexArrayObject {
public:
    VertexArrayObject(VertexBufferObject *VBO, ElementBufferObject *EBO, Shader *shader);
	
    void Bind();
    void Unbind();
	unsigned int GetVAO();
    VertexBufferObject *GetVBO();
    ElementBufferObject *GetEBO();
	size_t GetIndicesSize();
	size_t GetVerticesSize();

	~VertexArrayObject();
private:
    unsigned int VAO;
    VertexBufferObject *VBO;
    ElementBufferObject *EBO;
	Shader *shader;
	std::size_t indicesSize;
	std::size_t verticesSize;
    std::vector<glm::uvec4> locationList;

	void AddVertexAttributes(std::vector<t_vertexAttribute> &vertexAttributes);
};