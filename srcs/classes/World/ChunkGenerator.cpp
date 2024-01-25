#include <classes/World/ChunkGenerator.hpp>

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

u_char		*ChunkGenerator::generator(Chunk &chunk) {
	u_int sizeX = chunk.sizeX;
	u_int sizeY = chunk.sizeY;
	u_int sizeZ = chunk.sizeZ;

	int posX = chunk.GetX();
	int posY = chunk.GetY();
	u_char *data = (u_char*)calloc(sizeX * sizeY * sizeZ, sizeof(*data));

	int hill_height;

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
		hill_height = ground_height + (int)((hill_factor + ground_factor) * 30 + 5);
		
		while (z < hill_height) {
			data[x * sizeZ + z + y * sizeX * sizeZ] = STONE;
			z++;
		}

		//faire que ce soit perpendiculaire a l'inclinaison du sol
		double detail_factor = noiseList[2]->Octave2D(0.156 * p_x, 0.184 * p_y, 5, 0.2);
		int detail_height = hill_height + (int)((detail_factor + (hill_factor / 10)) * (6 * (detail_factor + detail_factor * detail_factor)));
		while (z < detail_height) {
			data[x * sizeZ + z + y * sizeX * sizeZ] = STONE;
			z++;
		}



		double dirt_factor = noiseList[2]->Octave2D(0.00356 * p_x, 0.00395 * p_y, 1, 0.5);
		int dirt_height = hill_height + (int)((dirt_factor) * 10);

		while (z < dirt_height) {
			data[x * sizeZ + z + y * sizeX * sizeZ] = DIRT;
			z++;
		}

		if (z > detail_height && z == dirt_height && dirt_factor > 0.1)
			data[x * sizeZ + z + y * sizeX * sizeZ] = GRASS;

	}
	}

	for (int y = 0; y < sizeY; y++) {
	for (int x = 0; x < sizeX; x++) {

		int z = 1;
		for ( ; z < hill_height * 0.8; z++) {
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