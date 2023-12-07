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

	u_char* 				GetAdjacentRuban(int x, int y, int z, int &pos, u_char direction);

	u_int* 					rubans_id;
	
	u_int 					rubansIndexes[16][16];

	u_int 					sizeData = 0;
  
public :
	static std::vector<std::vector<ChunkRLE*>>* loadedChunks;

	static constexpr char * shaderName = (char*)"default";

	static void 			setRenderDistance(int renderDistance);

	~ChunkRLE();
	ChunkRLE();
	ChunkRLE(int posX, int posY);

	u_int					GetRubanPos(int x, int y, int z);
	void					loadChunk();

	ChunkRLE*				GetNeighbour(int cardinal);



	void					BindNeighbour(int direction, ChunkRLE* chunk);
	
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