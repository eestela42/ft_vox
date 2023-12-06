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

	template <typename T>
    void AddVertexAttribute(u_int location, u_int size, T valueTemplate);

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
    std::vector<glm::uvec4> locationList;
    int totalStride = 0;
	
};

template <typename T>
void VertexArrayObject::AddVertexAttribute(u_int location, u_int amount, T valueTemplate) {
	if (typeid(valueTemplate) != typeid(float) && typeid(valueTemplate) != typeid(int)) {
		exit(1);
	}
	GLenum glType = GL_FLOAT;
	if (typeid(valueTemplate) == typeid(int)) {
		glType = GL_INT;
	}
	locationList.push_back(glm::uvec4(location, amount, sizeof(valueTemplate), glType));
   
    totalStride += amount * sizeof(valueTemplate);

    int tempStride = 0;
    for (std::vector<glm::uvec4>::iterator iterator = locationList.begin(); iterator != locationList.end(); iterator++) {
		if (iterator->w == GL_FLOAT) {
			glVertexAttribPointer(iterator->x, iterator->y, iterator->w, GL_FALSE, totalStride, (void *)(u_long)tempStride);
		}
		else if (iterator->w == GL_INT) {
			glVertexAttribIPointer(iterator->x, iterator->y, iterator->w, totalStride, (void *)(u_long)tempStride);
		}
        
        tempStride += iterator->y * iterator->z;
    }
    glEnableVertexAttribArray(location);
}