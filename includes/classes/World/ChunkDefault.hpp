#pragma once

#include <classes/World/Chunk.hpp>
#include <classes/VAO/VertexArrayObjectHandler.hpp>


class ChunkDefault : public Chunk
{
	public:
		ChunkDefault();
		ChunkDefault(u_int x, u_int y);
		bool isFilled(int x, int y, int z) override;
		bool IsRealNeighbor(int chunkX, int chunkY);
	private:
		std::vector<float> vertexData;
		void CreateCube(u_char faces, int x, int y, int z, int offsetX, int offsetY);
		
		void CompileData() override;
		void Generate() override;
		void Generate(std::vector<glm::ivec3> positionList, std::vector<glm::ivec3> sizeList) override;
		void Generate(u_int seed) override;
};