#ifndef RLE_HPP
# define RLE_HPP

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
# include <math.h>
# include <chrono>

# include <classes/World/Chunk.hpp>

	
class ChunkRLE : protected Chunk
{

protected :


	u_char* 				GetAdjacentRuban(int x, int y, int &pos, u_char direction);

	ChunkRLE* 				_neighbours[4]; // 0: North, 1: East, 2: South, 3: West

public :

	static constexpr char * shaderName = (char*)"default";

	~ChunkRLE();
	ChunkRLE();
	ChunkRLE(int posX, int posY);
	
	int 					calcX(int pos);
	int 					calcY(int pos);


	std::vector<float>&		GetVertexData();
	std::vector<u_int>&		GetShapeAssemblyData();

	void 					CompileData();

	void 					Generate();
	void 					Generate(std::vector<glm::ivec3> positionList,
											std::vector<glm::ivec3> sizeList);
	void 					Generate(u_int seed);

};



#endif