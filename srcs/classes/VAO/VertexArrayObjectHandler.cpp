#include <classes/VAO/VertexArrayObjectHandler.hpp>

std::mutex VertexArrayObjectHandler::vaoCreateQueueMutex;
std::vector<std::pair<t_vertexData *, std::vector<unsigned int> *>> VertexArrayObjectHandler::vaoCreateQueue;

std::mutex VertexArrayObjectHandler::vaoDeleteQueueMutex;
std::vector<u_int> VertexArrayObjectHandler::vaoDeleteQueue;

VertexArrayObjectHandler::VertexArrayObjectHandler() {}

VertexArrayObjectHandler::~VertexArrayObjectHandler() {
	vaoMapMutex.lock();
	for (auto const& x : vaoMap)
	{
		delete x.second;
	}
	vaoMapMutex.unlock();

}



void VertexArrayObjectHandler::Draw(u_int type) {
	switch (type) {
		case 0:
			DrawElements();
			break;
		case 1:
			DrawArray();
			break;
	}
}

void VertexArrayObjectHandler::DrawElements() {	
	if (activeVAO) {
		vaoMapMutex.lock();

		glDrawElements(GL_TRIANGLES, vaoMap[activeVAO - 1]->GetIndicesDataSize(), GL_UNSIGNED_INT, 0);	
		vaoMapMutex.unlock();
	}
}

void VertexArrayObjectHandler::DrawArray() {	
	if (activeVAO) {
		vaoMapMutex.lock();
		glDrawArrays(GL_POINTS, 0, vaoMap[activeVAO - 1]->GetIndicesSize());
		vaoMapMutex.unlock();
	}
}


void VertexArrayObjectHandler::DrawAll(u_int type) {
	// std::cout << "----DrawAll" << std::endl;
	switch (type) {
		case 0:
			DrawAllElements();
			break;
		case 1:
			DrawAllArray();
			break;
	}
	// std::cout << "---Exit DrawAll" << std::endl;
}

void VertexArrayObjectHandler::DrawAllElements() {
	Unbind();
	vaoMapMutex.lock();
	for (auto const& x : vaoMap)
	{

		x.second->Bind();
		glDrawElements(GL_TRIANGLES, x.second->GetIndicesDataSize(), GL_UNSIGNED_INT, 0);
		x.second->Unbind();
	}
	vaoMapMutex.unlock();
}

void VertexArrayObjectHandler::DrawAllArray() {
	Unbind();
	vaoMapMutex.lock();
	for (auto const& x : vaoMap)
	{

		x.second->Bind();
		glDrawArrays(GL_POINTS, 0, x.second->GetIndicesSize());
		x.second->Unbind();
	}
	vaoMapMutex.unlock();
}

bool isInFrustum(const glm::vec4& clipSpacePosition) {
    // Check if the position is inside the normalized device coordinates (NDC)
    if (clipSpacePosition.x >= -1.0f && clipSpacePosition.x <= 1.0f &&
        clipSpacePosition.y >= -1.0f && clipSpacePosition.y <= 1.0f) {
        return true;
    }
    return false;
}

u_int VertexArrayObjectHandler::addVaoCreateQueue(t_vertexData *vertexData, std::vector<unsigned int> *indices)
{
	std::cout << "Adding to create queue" << std::endl;
	vaoCreateQueueMutex.lock();
	vaoCreateQueue.push_back(std::make_pair(vertexData, indices));
	u_int to_ret = indexCount + vaoCreateQueue.size();
	vaoCreateQueueMutex.unlock();
	std::cout << "EXIT Adding to create queue" << std::endl;

	return to_ret;

}

void VertexArrayObjectHandler::addVaoDeleteQueue(u_int vao)
{
	std::cout << "Adding to delete queue" << std::endl;

	vaoDeleteQueueMutex.lock();
	vaoDeleteQueue.push_back(vao);
	vaoDeleteQueueMutex.unlock();
	std::cout << "Exit Adding to delete queue" << std::endl;

}

void VertexArrayObjectHandler::vaoCreate(ShaderHandler *shaderHandler)
{
	vaoCreateQueueMutex.lock();
	Shader *shader = shaderHandler->GetShader(ChunkRLE::shaderName);
	while (vaoCreateQueue.size())
	{

		VertexArrayObject *vao = new VertexArrayObject(new VertexBufferObject(*vaoCreateQueue[0].first), new ElementBufferObject(*vaoCreateQueue[0].second), shader);
		AddVAO(vao);
		vaoCreateQueue.erase(vaoCreateQueue.begin());
	}
	vaoCreateQueueMutex.unlock();

}

void VertexArrayObjectHandler::vaoDelete()
{
	vaoDeleteQueueMutex.lock();
	while (vaoDeleteQueue.size())
	{
		RemoveVAO(vaoDeleteQueue[0]);
		vaoDeleteQueue.erase(vaoDeleteQueue.begin());
	}
	vaoDeleteQueueMutex.unlock();
}

u_int  VertexArrayObjectHandler::AddVAO(VertexArrayObject *vao) {
	indexCount++;
	vaoMapMutex.lock();
    vaoMap[indexCount] = vao;
	vaoMapMutex.unlock();
	activeVAO = indexCount;
	return indexCount;
}

void VertexArrayObjectHandler::RemoveVAO(u_int VAO) {
	vaoMapMutex.lock();
	delete vaoMap[VAO];
	vaoMap.erase(VAO);
	vaoMapMutex.unlock();
	if (VAO == activeVAO) {
		activeVAO = 0;
	}
}

void VertexArrayObjectHandler::Bind(u_int VAO) {
	if (VAO && activeVAO != VAO) {
		vaoMapMutex.lock();
		if (activeVAO) {
			vaoMap[activeVAO]->Unbind();
		}
		vaoMap[VAO]->Bind();
		vaoMapMutex.unlock();
		activeVAO = VAO;
	}
}

void VertexArrayObjectHandler::Unbind() {
	if (activeVAO) {
		vaoMapMutex.lock();
		vaoMap[activeVAO]->Unbind();
		vaoMapMutex.unlock();
	}
	glBindVertexArray(0);
}

VertexArrayObject *VertexArrayObjectHandler::GetVAO(u_int VAO) {
	return (vaoMap[VAO]);
}
