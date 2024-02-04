#include <classes/World/ChunkGenerator.hpp>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>

std::vector<PerlinNoise*> 		ChunkGenerator::noiseList;

void ChunkGenerator::pushBackNoiseList(PerlinNoise* tmp)
{
	ChunkGenerator::noiseList.push_back(tmp);
}

ChunkGenerator::~ChunkGenerator()
{
	
}

ChunkGenerator::ChunkGenerator()
{
	
}

ChunkGenerator::ChunkGenerator(u_int seed)
{
	
}

void ChunkGenerator::generateTree(u_char *data, int x, int y, int z) {
	int tronc = z;
	for (; tronc < z + 4; tronc++) {
		data[x * 256 + tronc + y * 16 * 256] = OAK_WOOD_SIDE;
	}
	data[x * 256 + tronc + y * 16 * 256] = MOSSY_COBBLESTONE;
	data[x * 256 + tronc + 1 + y * 16 * 256] = MOSSY_COBBLESTONE;

	if (x > 0)
	data[(x - 1) * 256 + tronc + y * 16 * 256] = MOSSY_COBBLESTONE;
	if (x < 15)
	data[(x + 1) * 256 + tronc + y * 16 * 256] = MOSSY_COBBLESTONE;
	if (y > 0)
	data[x * 256 + tronc + (y - 1) * 16 * 256] = MOSSY_COBBLESTONE;
	if (y < 15)
	data[x * 256 + tronc + (y + 1) * 16 * 256] = MOSSY_COBBLESTONE;
	
}

u_char		*ChunkGenerator::generator(Chunk &chunk) {
	u_int sizeX = chunk.sizeX;
	u_int sizeY = chunk.sizeY;
	u_int sizeZ = chunk.sizeZ;

	int posX = chunk.GetX();
	int posY = chunk.GetY();
	u_char *data = (u_char*)calloc(sizeX * sizeY * sizeZ, sizeof(*data));

	int hill_height;
	std::default_random_engine engine(389 * posX * posY);

	for (int y = 0; y < sizeY; y++) {
	for (int x = 0; x < sizeX; x++) {

		data[x * sizeZ + y * sizeX * sizeZ] = BEDROCK;

		int z = 1;

		double p_x = ((double)posX * sizeX + x);
		double p_y = ((double)posY * sizeY + y);
		
		double ground_factor = noiseList[0]->Octave2D(0.00356 * p_x, 0.00395 * p_y, 3, 0.5);
		int ground_height = (int)(ground_factor * 60 + 40);

		while (z < ground_height) {
			data[x * sizeZ + z + y * sizeX * sizeZ] = STONE;
			z++;
		}

		double hill_factor = noiseList[1]->Octave2D(0.0158 * p_x, 0.01568 * p_y, 3, 0.5);
		hill_height = ground_height + (int)((hill_factor + ground_factor) * 70 + 5);
		
		while (z < hill_height) {
			data[x * sizeZ + z + y * sizeX * sizeZ] = STONE;
			z++;
		}

		//faire que ce soit perpendiculaire a l'inclinaison du sol
		double detail_factor =  0.20 + noiseList[2]->Octave2D(0.206 * p_x, 0.204 * p_y, 5, 0.4);
		// int detail_height = hill_height + (int)((detail_factor + (hill_factor / 10)) * (6 * (detail_factor / 2 + detail_factor * detail_factor)));
		int detail_height = hill_height + std::pow(detail_factor, 5) * 6;
		
		while (z < detail_height) {
			data[x * sizeZ + z + y * sizeX * sizeZ] = STONE;
			z++;
		}



		double dirt_factor = noiseList[2]->Octave2D(0.00356 * p_x, 0.00395 * p_y, 1, 0.5);
		int dirt_height = hill_height + (int)((dirt_factor) * 12);

		while (z < dirt_height) {
			data[x * sizeZ + z + y * sizeX * sizeZ] = DIRT;
			z++;
		}

	
		if (z > detail_height && z == dirt_height && dirt_factor > 0.1)
		{
			data[x * sizeZ + z - 1 + y * sizeX * sizeZ] = GRASS;
			if (engine.operator()() % 100 == 0)
			{
				generateTree(data, x, y, z - 1);
			}
		}

	}
	}
	// return data;



	for (int y = 0; y < sizeY; y++) {
	for (int x = 0; x < sizeX; x++) {
		double p_x = ((double)posX * sizeX + x);
		double p_y = ((double)posY * sizeY + y);

		int z = 1;
		for ( ; z < hill_height /* * noiseList[4]->Octave2D(0.00056 * p_x, 0.00045 * p_y, 1, 0.5)*/; z++) {
			double cave_factor = noiseList[3]->Octave3D(0.0356 * (posX * sizeX + x), 0.0395 * (posY * sizeY + y), z * 0.13, 1, 0.5);
			if (cave_factor > 0.6) {
				data[x * sizeZ + z + y * sizeX * sizeZ] = AIR;
				continue ;
			}
			if (cave_factor < 0.58442 && cave_factor > 0.58438) {
				data[x * sizeZ + z + y * sizeX * sizeZ] = GOLD_MINERAL;
			}
			if (cave_factor < 0.582201 && cave_factor > 0.5815) {
				data[x * sizeZ + z + y * sizeX * sizeZ] = IRON_MINERAL;
			}
		}
	}
	}
	return data;
}