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

	u_char *data;
	bool isCompiled = false;
	std::vector<u_char> vertexData;
	std::vector<u_int> shapeAssemblyData;

	void DataCompiler();
public :
	static u_int const sizeX = 16;
	static u_int const sizeY = 16;
	static u_int const sizeZ = 256;

	Chunk();

	std::vector<u_char>&	GetVertexData();
	std::vector<u_int>&		GetShapeAssemblyData();

	void Generate();
	void Generate(std::vector<glm::ivec3> positionList, std::vector<glm::ivec3> sizeList);
	void Generate(u_int seed);

	~Chunk();
};

#endif