#pragma once

#include <classes/VAO/VertexArrayObject.hpp>
#include <unordered_map>

class VertexArrayObjectHandler {
public:
	VertexArrayObjectHandler();
    ~VertexArrayObjectHandler();

	void Draw();
	void DrawAll();
	VertexArrayObject *GetVAO(u_int VAO);
	
	u_int AddVAO(VertexArrayObject*);
	void RemoveVAO(u_int VAO);
	void Bind(u_int VAO);
	void Unbind();
private:
	std::unordered_map<u_int, VertexArrayObject*> vaoMap;
	u_int activeVAO = 0;
	u_int indexCount = 0;
};