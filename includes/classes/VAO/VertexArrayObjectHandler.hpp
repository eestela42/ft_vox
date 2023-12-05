#pragma once

#include <classes/VAO/VertexArrayObject.hpp>

class VertexArrayObjectHandler {
public:
	VertexArrayObjectHandler();
    ~VertexArrayObjectHandler();

	void Draw();
	VertexArrayObject *GetVAO(u_int VAO);
	u_int AddVAO(VertexArrayObject*);
	void Bind(u_int VAO);
private:
	std::vector<VertexArrayObject*> vaoList;
	std::vector<u_int> vaoListIndices;
	u_int activeVAO = 0;
};