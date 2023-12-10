#pragma once

#include <classes/World/ChunkDefault.hpp>
#include <classes/VAO/VertexArrayObjectHandler.hpp>
#include <unordered_map>

class ChunkInstantiator
{
	private:
		std::unordered_map<Chunk*, u_int> chunkMap;
		VertexArrayObjectHandler *vertexArrayObjectHandler;
		int playerChunkPosX;
		int playerChunkPosY;
		int renderDistance;
	public:
		ChunkInstantiator(VertexArrayObjectHandler *vertexArrayObjectHandler, int renderDistance);

		void Update(glm::vec3 playerPos);
};