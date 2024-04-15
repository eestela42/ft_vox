#pragma once

#include <classes/VAO/VertexArrayObject.hpp>
#include <unordered_map>
#include <classes/Text.hpp>

class VertexArrayObjectHandler {
public:
	VertexArrayObjectHandler();
    ~VertexArrayObjectHandler();

	void Draw(u_int vaoIndex);
	void Draw(Text *text);
	void DrawAll();
	VertexArrayObject *GetVAO(u_int VAO);
	
	u_int AddWorldVAO(VertexArrayObject*);
	void RemoveWorldVAO(u_int VAO);
	void ReplaceTextVAO(Text *text, VertexArrayObject *vao);
	void AddTextVAO(Text *, VertexArrayObject *);
	void RemoveTextVAO(Text*);
private:
	void Draw(VertexArrayObject *toDraw);
	std::unordered_map<u_int, VertexArrayObject *> worldVaoMap;
	std::unordered_map<Text*, VertexArrayObject*> textVaoMap;
	u_int indexCount = 0;
};