#include <classes/VAO/VertexArrayObjectHandler.hpp>

VertexArrayObjectHandler::VertexArrayObjectHandler() {}

void VertexArrayObjectHandler::Draw() {
	if (activeVAO) {
		glDrawElements(GL_TRIANGLES, vaoList[activeVAO - 1]->GetIndicesSize(), GL_UNSIGNED_INT, 0);
	}
}

u_int VertexArrayObjectHandler::AddVAO(VertexArrayObject *vao) {
    vaoList.push_back(vao);
    u_int index = vaoList.size();
    vaoListIndices.push_back(vao->GetVAO());
    return index;
}

void VertexArrayObjectHandler::Bind(u_int VAO) {
	if (VAO && activeVAO != VAO && VAO <= vaoListIndices.size()) {
		if (activeVAO) {
			vaoList[activeVAO - 1]->Unbind();
		}
		vaoList[VAO - 1]->Bind();
		activeVAO = VAO;
	}
}

void VertexArrayObjectHandler::Unbind() {
	if (activeVAO) {
		vaoList[activeVAO - 1]->Unbind();
	}
	glBindVertexArray(0);
}

VertexArrayObject *VertexArrayObjectHandler::GetVAO(u_int VAO) {
	return (vaoList[VAO - 1]);
}

VertexArrayObjectHandler::~VertexArrayObjectHandler() {}