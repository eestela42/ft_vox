#pragma once

#include <classes/World/ChunkDefault.hpp>
#include <classes/VAO/VertexArrayObjectHandler.hpp>
#include <classes/ShaderHandler.hpp>
#include <unordered_map>

class ChunkInstantiator
{
	private:
		std::unordered_map<Chunk*, u_int> chunkMap;
		VertexArrayObjectHandler *vertexArrayObjectHandler;
		int playerChunkPosX;
		int playerChunkPosY;
		int renderDistance;
		ShaderHandler *shaderHandler;
	public:
		ChunkInstantiator(VertexArrayObjectHandler *vertexArrayObjectHandler, int renderDistance, ShaderHandler *shaderHandler);

		void Update(glm::vec3 playerPos);
};