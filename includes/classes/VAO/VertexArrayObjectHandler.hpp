#pragma once

#include <classes/VAO/VertexArrayObject.hpp>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <thread>
#include <mutex>
#include <classes/ShaderHandler.hpp>
#include <classes/World/ChunkRLE.hpp>


class VertexArrayObjectHandler {
public:
	static std::mutex vaoCreateQueueMutex;
	static std::vector<std::pair<t_vertexData *, std::vector<unsigned int> *>> vaoCreateQueue;

	static std::mutex vaoDeleteQueueMutex;
	static std::vector<u_int> vaoDeleteQueue;
	VertexArrayObjectHandler();
	~VertexArrayObjectHandler();

	void Draw(u_int type);
	void DrawElements();
	void DrawArray();
	
	void DrawAll(u_int type);
	void DrawAllElements();
	void DrawAllArray();

	VertexArrayObject *GetVAO(u_int VAO);
	
	u_int addVaoCreateQueue(t_vertexData *vertexData, std::vector<unsigned int> *indices);
	void addVaoDeleteQueue(u_int vao);

	void vaoCreate(ShaderHandler *shaderHandler);
	void vaoDelete();

	u_int AddVAO(VertexArrayObject*);
	void RemoveVAO(u_int VAO);
	void Bind(u_int VAO);
	void Unbind();
private:
	std::mutex vaoMapMutex;
	std::unordered_map<u_int, VertexArrayObject*> vaoMap;
	u_int activeVAO = 0;
	u_int indexCount = 0;
};