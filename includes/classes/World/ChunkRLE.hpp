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

	
class ChunkRLE : public Chunk
{
private:
	std::vector<float> vertexData;

protected :

	u_char* 				GetAdjacentRuban(int x, int y, int z, int &pos, u_char direction);

	u_int* 					rubans_id;
	
	u_int 					rubansIndexes[16][16];

	u_int 					sizeData = 0;
  
public :


	static constexpr char * shaderName = (char*)"RLE";


	~ChunkRLE();
	ChunkRLE();
	ChunkRLE(int posX, int posY);

	void 					createPointVertex(std::vector<int> &vertexes, int pos, u_char orientation, u_char type);

	void 					CreateFaceRLE(int oreientation, std::vector<int> &vData, std::vector<u_int> &iData,
												int x, int y, int z, int offset, u_char type);
	bool					isFilled(int x, int y, int z) override;
	u_int					GetRubanPos(int x, int y, int z);
	void					loadChunk();

	ChunkRLE*				GetNeighbour(int cardinal);



	void					BindNeighbour(int direction, ChunkRLE* chunk);
	
	int 					calcX(int pos);
	int 					calcY(int pos);

	void 					CompileData() override;

	void 					Generate() override;
	void 					GenerateTest(PerlinNoise *noise, PerlinNoise *noise2);
	void 					Generate(float *gen, int start);

	void 					Generate(std::vector<glm::ivec3> positionList,
											std::vector<glm::ivec3> sizeList) override;
	void 					Generate(u_int seed) override;

};



#endif