#ifndef CHUNKGENERATOR_HPP
# define CHUNKGENERATOR_HPP

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
# include <math.h>
# include <chrono>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

#include <classes/World/ChunkDefault.hpp>
#include <classes/World/ChunkRLE.hpp>
#include <classes/World/PerlinNoise.hpp>


class ChunkGenerator
{
	private :

		
	public :
	
	static std::vector<PerlinNoise*> 		noiseList;

	void pushBackNoiseList(PerlinNoise* tmp);

	u_char *data;
	
	int posX;
	int posY;
	u_int sizeX;
	u_int sizeY;
	u_int sizeZ;

	int ground_height;
	int hill_height;
	double p_x;
	double p_y;

	std::default_random_engine engine;

		~ChunkGenerator();
		ChunkGenerator();
		ChunkGenerator(u_int seed);
		
		u_char*		generator(Chunk &chunk);

		void		generateTree(int &x, int &y, int &z);
		int 		genBedrock(u_char *data);
		
		int			genUnderLayer(int pos, int &z);
		int			genOverLayer( int pos, int &z);

		int 		gen2DCave(int hill_height, int pos, int &z);

		int 		genWater(int pos, int &z);
		int 		gen3DCave(int hill_height, int pos, int &z);
};

#endif