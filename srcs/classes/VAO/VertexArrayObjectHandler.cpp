#include <classes/VAO/VertexArrayObjectHandler.hpp>

VertexArrayObjectHandler::VertexArrayObjectHandler() {}

void VertexArrayObjectHandler::Draw() {
	if (activeVAO) {
		glDrawElements(GL_TRIANGLES, vaoMap[activeVAO - 1]->GetIndicesSize(), GL_UNSIGNED_INT, 0);
	}
}

void VertexArrayObjectHandler::DrawAll() {
	Unbind();

	for (auto const& x : vaoMap)
	{
		

		x.second->Bind();
		glDrawElements(GL_TRIANGLES, x.second->GetIndicesSize(), GL_UNSIGNED_INT, 0);
		x.second->Unbind();
	}
}

bool isInFrustum(const glm::vec4& clipSpacePosition) {
    // Check if the position is inside the normalized device coordinates (NDC)
    if (clipSpacePosition.x >= -1.0f && clipSpacePosition.x <= 1.0f &&
        clipSpacePosition.y >= -1.0f && clipSpacePosition.y <= 1.0f) {
        return true;
    }
    return false;
}


void VertexArrayObjectHandler::DrawAll(glm::vec3 originCameraPosition, glm::vec3 originCameraDirection) {
    Unbind();

    // Calculate view matrix
    glm::mat4 viewMatrix = glm::lookAt(originCameraPosition, originCameraPosition + originCameraDirection, glm::vec3(0.0f, 1.0f, 0.0f));

    // Calculate projection matrix (assuming perspective projection)
    float aspectRatio = 1540 / 840; // Adjust with your actual screen dimensions
    float fov = glm::radians(45.0f); // Adjust with your desired field of view
    float nearPlane = 0.1f; // Adjust with your near plane distance
    float farPlane = 100.0f; // Adjust with your far plane distance
    glm::mat4 projectionMatrix = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

    // Calculate view-projection matrix
    glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;

    for (auto const& x : vaoMap) {
        // Get VAO position (assuming x and y coordinates form the position)
        glm::vec3 vaoPosition = glm::vec3(x.second->posX * 16, x.second->posY * 16, 0.0f);

        // Transform VAO position to clip space
        glm::vec4 clipSpacePosition = viewProjectionMatrix * glm::vec4(vaoPosition, 1.0f);

        // Perform frustum culling
        if (isInFrustum(clipSpacePosition)) {
			std::cout << "not in frustum " << x.second->posX << " " << x.second->posY  << std::endl;
            continue; // VAO is not in front of the camera, skip rendering
        }

        // Bind VAO and draw
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