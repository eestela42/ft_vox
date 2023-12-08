#ifndef CHUNK_HPP
# define CHUNK_HPP


#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>


class Chunk
{
private:
	Chunk();

	bool didUpdate = false;
	bool isCompiled = false;
	bool isGenerated = false;

	Chunk *GetNeighbor(int x, int y);
	void SetReady();
	void loadChunk();
	virtual void CompileData() = 0;
	virtual void Generate() = 0;
	virtual void Generate(std::vector<glm::ivec3> positionList, std::vector<glm::ivec3> sizeList) = 0;
	virtual void Generate(u_int seed) = 0;
protected :
	static std::vector<std::vector<Chunk*>> loadedChunks;

	Chunk(int posX, int posY);
	
	Chunk* neighborChunks[4] = {0};
	u_char *data = (u_char*)""; //Warning, this will cause segfault if modified before instantiation
	std::vector<float> vertexData;
	std::vector<u_int> shapeAssemblyData;
	int posX;
	int posY;
	
	bool IsGenerated();

public :
	/** @brief Not yet ready to be sized down dynamically without neighbor loss issues*/
	static void setRenderDistance(int renderDistance);
	static constexpr char * shaderName = (char*)"default";
	static u_int const sizeX = 16;
	static u_int const sizeY = 16;
	static u_int const sizeZ = 256;

	virtual bool isFilled(int x, int y, int z) = 0;
	bool IsRealNeighbor(int chunkX, int chunkY);
	virtual void PublicGenerate() final;
	virtual void PublicGenerate(std::vector<glm::ivec3> positionList, std::vector<glm::ivec3> sizeList) final;
	virtual void PublicGenerate(u_int seed) final;

	virtual std::vector<float>&	GetVertexData() final;
	virtual std::vector<u_int>&	GetShapeAssemblyData() final;
	virtual bool DidUpdate() final;

	virtual ~Chunk() = 0;
};

#endif