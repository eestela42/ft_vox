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
	std::cout << "Generating chunk " << chunk.GetX() << " " << chunk.GetY() << std::endl;

	int posX = chunk.GetX();
	int posY = chunk.GetY();
	u_char *data = (u_char*)calloc(sizeX * sizeY * sizeZ, sizeof(*data));
	for (int y = 0; y < sizeY; y++) {
	for (int x = 0; x < sizeX; x++) {

		data[x * sizeZ + y * sizeX * sizeZ] = BEDROCK;
		double p_x = ((double)posX * sizeX + x);
		double p_y = ((double)posY * sizeY + y);
		
		double v1 = noiseList[0]->Octave2D(0.00456 * p_x, 0.00395 * p_y, 5, 0.5);
		int z = 1;
		for (; z < v1 * 80; z++) 
		{
				data[z + x * sizeZ + y * sizeX * sizeZ] = STONE;
		}
		data[z + x * sizeZ + y * sizeX * sizeZ] = GRASS;
	}
	}
	std::cout << "end" << std::endl;
	return data;
}