#ifndef CHUNK_HPP
# define CHUNK_HPP


#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>


class Chunk
{

protected :
	int posX;
	int posY;

	u_char *data = (u_char*)""; //Warning, this will cause segfault if modified before instantiation
	bool isCompiled = false;
	std::vector<float> vertexData;
	std::vector<u_int> shapeAssemblyData;

	void CompileData();
public :
	static constexpr char * shaderName = (char*)"default";
	static u_int const sizeX = 16;
	static u_int const sizeY = 16;
	static u_int const sizeZ = 256;

	Chunk();
	Chunk(u_int x, u_int y);

	std::vector<float>&	GetVertexData();
	std::vector<u_int>&		GetShapeAssemblyData();

	void Generate();
	void Generate(std::vector<glm::ivec3> positionList, std::vector<glm::ivec3> sizeList);
	void Generate(u_int seed);

	~Chunk();
};

#endif