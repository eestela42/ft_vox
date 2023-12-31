#include <classes/VAO/VertexArrayObjectHandler.hpp>

VertexArrayObjectHandler::VertexArrayObjectHandler() {}

void VertexArrayObjectHandler::Draw() {
	if (activeVAO) {
		glDrawElements(GL_TRIANGLES, vaoMap[activeVAO - 1]->GetIndicesSize(), GL_UNSIGNED_INT, 0);
	}
}

void VertexArrayObjectHandler::DrawAll() {
	Unbind();
	int i = 0;
	for (auto const& x : vaoMap)
	{
		x.second->Bind();
		glDrawElements(GL_TRIANGLES, x.second->GetIndicesSize(), GL_UNSIGNED_INT, 0);
		x.second->Unbind();
	}
}

u_int VertexArrayObjectHandler::AddVAO(VertexArrayObject *vao) {
	indexCount++;
    vaoMap[indexCount] = vao;
	activeVAO = indexCount;
    return indexCount;
}

void VertexArrayObjectHandler::RemoveVAO(u_int VAO) {
	delete vaoMap[VAO];
	vaoMap.erase(VAO);
	if (VAO == activeVAO) {
		activeVAO = 0;
	}
}

void VertexArrayObjectHandler::Bind(u_int VAO) {
	if (VAO && activeVAO != VAO) {
		if (activeVAO) {
			vaoMap[activeVAO]->Unbind();
		}
		vaoMap[VAO]->Bind();
		activeVAO = VAO;
	}
}

void VertexArrayObjectHandler::Unbind() {
	if (activeVAO) {
		vaoMap[activeVAO]->Unbind();
	}
	glBindVertexArray(0);
}

VertexArrayObject *VertexArrayObjectHandler::GetVAO(u_int VAO) {
	return (vaoMap[VAO]);
}

VertexArrayObjectHandler::~VertexArrayObjectHandler() {}