#include <classes/VAO/VertexArrayObjectHandler.hpp>

VertexArrayObjectHandler::VertexArrayObjectHandler() {}

void VertexArrayObjectHandler::Draw(VertexArrayObject *toDraw) {
	toDraw->Bind();
	if (toDraw->GetEBO()) {
		glDrawElements(GL_TRIANGLES, toDraw->GetIndicesSize(), GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_POINTS, 0, toDraw->GetVerticesSize()/12);
	}
	toDraw->Unbind();
}

void VertexArrayObjectHandler::Draw(Text *text) {
	text->SetUniforms();
	Draw(textVaoMap[text]);
}

void VertexArrayObjectHandler::Draw(u_int vaoIndex) {
	Draw(worldVaoMap[vaoIndex]);
}

void VertexArrayObjectHandler::DrawAll() {
	int i = 0;
	for (auto const& x : worldVaoMap)
	{
		Draw(x.second);
	}
	for (auto const& x : textVaoMap)
	{
		if (x.first->HasUpdated()) {
			ReplaceTextVAO(x.first, new VertexArrayObject(new VertexBufferObject(x.first->GetVertexData()), 0, x.first->GetShader()));
		}
		x.first->SetUniforms();
		Draw(x.second);
	}
}

u_int VertexArrayObjectHandler::AddWorldVAO(VertexArrayObject *vao) {
	indexCount++;
    worldVaoMap[indexCount] = vao;
    return indexCount;
}

void VertexArrayObjectHandler::RemoveWorldVAO(u_int VAO) {
	delete worldVaoMap[VAO];
	worldVaoMap.erase(VAO);
}

void VertexArrayObjectHandler::ReplaceTextVAO(Text *text, VertexArrayObject *vao) {
	delete textVaoMap[text];
	textVaoMap[text] = vao;
}

void VertexArrayObjectHandler::AddTextVAO(Text *text, VertexArrayObject *vao)
{
	textVaoMap[text] = vao;
}

void VertexArrayObjectHandler::RemoveTextVAO(Text *text)
{
	delete textVaoMap[text];
	textVaoMap.erase(text);
}

VertexArrayObject *VertexArrayObjectHandler::GetVAO(u_int VAO) {
	return (worldVaoMap[VAO]);
}

VertexArrayObjectHandler::~VertexArrayObjectHandler() {}