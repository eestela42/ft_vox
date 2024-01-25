#ifndef CHUNKGENERATOR_HPP
# define CHUNKGENERATOR_HPP

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
# include <math.h>
# include <chrono>

#include <classes/World/ChunkDefault.hpp>
#include <classes/World/ChunkRLE.hpp>
#include <classes/World/PerlinNoise.hpp>


class ChunkGenerator
{
	private :

		
	public :
	
	static std::vector<PerlinNoise*> 		noiseList;

	void pushBackNoiseList(PerlinNoise* tmp);


		~ChunkGenerator();
		ChunkGenerator();
		ChunkGenerator(u_int seed);
		
		void		generateTree(u_char *data, int x, int y, int z);
		u_char*		generator(Chunk &chunk);
};

#endif