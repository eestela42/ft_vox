#include <classes/VAO/VertexArrayObjectHandler.hpp>

VertexArrayObjectHandler::VertexArrayObjectHandler() {}

void VertexArrayObjectHandler::Draw() {
	if (activeVAO) {
		glDrawElements(GL_TRIANGLES, vaoMap[activeVAO - 1]->GetIndicesSize(), GL_UNSIGNED_INT, 0);
	}
}

void VertexArrayObjectHandler::DrawAll(glm::vec3 originCameraPosition, glm::vec3 originCameraDirection) {
	Unbind();
	int i = 0;
	glm::vec2 cameraDirection = glm::normalize(glm::vec2(originCameraDirection.x, originCameraDirection.z));
	glm::vec2 cameraPosition =  glm::normalize(glm::vec2(originCameraPosition.x, originCameraPosition.z));
	// cameraPosition -= cameraDirection * 10.0f;
	for (auto const& x : vaoMap)
	{

		// glm::vec2 VAO_direction = glm::normalize(glm::vec2(x.second->posX, x.second->posY)) - cameraPosition;

		// VAO_direction = glm::normalize(VAO_direction);
		
		// if (glm::dot(VAO_direction, cameraDirection) < 0.0f)
		// 	{
		// 		continue;
		// 	}

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