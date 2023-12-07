#pragma once

#include <classes/World/Chunk.hpp>

class ChunkDefault : public Chunk
{
	public:
		ChunkDefault();
		ChunkDefault(u_int x, u_int y);
		bool isFilled(int x, int y, int z) override;
	private:
		void CreateCube(std::vector<float> &vData, std::vector<u_int> &iData, int x, int y, int z, int offset, int offsetX, int offsetY);
		
		void CompileData() override;
		void Generate() override;
		void Generate(std::vector<glm::ivec3> positionList, std::vector<glm::ivec3> sizeList) override;
		void Generate(u_int seed) override;
};