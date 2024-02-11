#include <classes/World/ChunkGenerator.hpp>


std::vector<PerlinNoise*> 		ChunkGenerator::noiseList;
std::map<position, std::vector<u_char>*> ChunkGenerator::modifMap;



ChunkGenerator::~ChunkGenerator()
{
}

void ChunkGenerator::initNoise(u_int seed)
{
	PerlinNoise *noise0 = new PerlinNoise(seed);
	PerlinNoise *noise1 = new PerlinNoise(seed + 13);

	PerlinNoise *noise2 = new PerlinNoise(seed + 59);
	PerlinNoise *noise3 = new PerlinNoise(seed + 42);
	PerlinNoise *noise4 = new PerlinNoise(seed + 53);
	PerlinNoise *noise5 = new PerlinNoise(seed + 23);
	PerlinNoise *noise6 = new PerlinNoise(seed + 17);
	PerlinNoise *noise7 = new PerlinNoise(seed + 19);

	ChunkGenerator init;
	ChunkGenerator::noiseList.push_back(noise0);
	ChunkGenerator::noiseList.push_back(noise1);
	ChunkGenerator::noiseList.push_back(noise2);
	ChunkGenerator::noiseList.push_back(noise3);
	ChunkGenerator::noiseList.push_back(noise4);
	ChunkGenerator::noiseList.push_back(noise5);
	ChunkGenerator::noiseList.push_back(noise6);
	ChunkGenerator::noiseList.push_back(noise7);
}

ChunkGenerator::ChunkGenerator()
{
	
}

ChunkGenerator::ChunkGenerator(u_int seed)
{
	
}

void ChunkGenerator::generateTree(int x, int y, int z) {
	int tronc = z - 1;
	for (; tronc < z + 4; tronc++) {
		data[x * 256 + tronc + y * 16 * 256] = OAK_WOOD_SIDE;
	}
	for (int i = -1; i < 2; i++) {
	for (int j = -1; j < 2; j++) {
	for (int k = 0; k < 2; k++) {
		if (x+i >= 0 && x+i <= 15 && y+j >= 0 && y+j <= 15 && tronc+k >= 0 && tronc+k < 255)
			data[(x+i) * 256 + (tronc+k) + (y+j) * 16 * 256] = MOSSY_COBBLESTONE;
	}
	}
	}
	if (tronc + 2 < 255)
		data[x * 256 + tronc + 2 + y * 16 * 256] = MOSSY_COBBLESTONE;
	
}

int ChunkGenerator::genBedrock(u_char *data, int x, int y)
{
	
	double bedrock_factor = 0;
	int bedHigh = 1 + (int)(bedrock_factor * 5);
	int k = 0;
	data[x * sizeZ + y * sizeX * sizeZ] = BEDROCK;
	for (; k < bedHigh; k++)
	{
		data[x * sizeZ + y * sizeX * sizeZ + k] = BEDROCK;
	}
	return k;
}

int ChunkGenerator::genUnderLayer(int pos, int &z)
{
		double ground_factor = noiseList[0]->Octave2D(0.001356 * p_x, 0.00195 * p_y, 3, 0.67);
		if (ground_factor > 0.5)
			ground_factor *= 1.0f + (ground_factor - 0.5f) * 2;
		ground_height = (int)(ground_factor * 90);

		bool isWater = false;
		if (ground_factor < 0.25)
		{
			isWater = true;
		}

		while (z < ground_height) {
			data[pos + z] = STONE;
			z++;
		}

		double hill_factor = noiseList[1]->Octave2D(0.00758 * p_x, 0.007568 * p_y, 4, 0.5);
		hill_height = ground_height + (int)((hill_factor * ground_factor) * 80 + 5);
		
		if (ground_factor + hill_factor  < 1.12)
		{
			while (z < hill_height) {
				data[pos + z] = STONE;
				z++;
			}
		}
		else
		{
			int air_end = 0;
			for ( ; z < hill_height ; z++)
			{
				double montain_factor = noiseList[3]->Octave3D(0.0356 * p_x, 0.0395 * p_y, z * 0.023, 2, 0.56);
				if (montain_factor < float(z)/(hill_height*2))
				{
					air_end++;
					data[pos + z] = AIR;
					continue ;
				}
				air_end = 0;
				data[pos + z] = STONE;
			}
			z -= air_end;
			hill_height = z;
		}

	return 0;
}

int ChunkGenerator::genOverLayer( int pos, int &z)
{
	double detail_factor =  0.20 + noiseList[2]->Octave2D(0.206 * p_x, 0.204 * p_y, 5, 0.4);
		int detail_height = hill_height + detail_factor * 6;
		
		while (z < detail_height) {
			data[pos + z] = STONE;
			z++;
		}



		double dirt_factor = noiseList[2]->Octave2D(0.00356 * p_x, 0.00395 * p_y, 2, 0.5);
		int dirt_height = hill_height + (int)((dirt_factor) * 12);

		u_char under_layer = DIRT;
		u_char over_layer = GRASS;

		double temperature = noiseList[5]->Octave2D(0.0002887 * p_x, 0.0002689 * p_y, 5, 0.7);

		if (hill_height > 110 + (temperature * 50))
			over_layer = IRON_BLOCK;
		if (temperature > 0.6)
		{
			under_layer = SAND;
			over_layer = SAND;
		}
		
		while (z < dirt_height) {
			data[pos + z] = under_layer;
			z++;
		}

	
		if (temperature < 0.6 && z > 60 && z == dirt_height && dirt_factor > 0.1)
		{
			double tree_factor = noiseList[3]->Octave2D(0.00856 * p_x, 0.00665 * p_y, 4, 0.4);
			tree_factor /= 10;
			data[pos + z -1] = over_layer;
			if (over_layer == GRASS && (engine.operator()() % 1000) / tree_factor < 1)
			{
				int x = pos / sizeZ % sizeX;
				int y = pos / (sizeZ * sizeX);
				if (x > 2 && x < 13 && y > 2 && y < 13)
					generateTree(x, y, z);
			}
		}
		return 0;
}

int ChunkGenerator::gen2DCave(int hill_height, int pos, int &z)
{
	double tunel = noiseList[5]->Octave2D(0.0076 * p_x, 0.0065 * p_y, 3, 0.3);
		if ((tunel > 0.33 && tunel < 0.36) || (tunel > 0.64 && tunel < 0.67) || (tunel > 0.94 && tunel < 0.97))
		{
			int start = noiseList[6]->Octave2D(0.0056 * p_x, 0.0045 * p_y, 3, 0.3) * (hill_height - 8);
			int size = noiseList[7]->Octave2D(0.0126 * p_x, 0.0135 * p_y, 4, 0.5) * 9;
			for (z = start; z < start + size; z++)
			{
				data[pos + z] = AIR;
			}
		}
	tunel = noiseList[6]->Octave2D(0.0076 * p_x, 0.0065 * p_y, 3, 0.3);
		if ((tunel > 0.33 && tunel < 0.36) || (tunel > 0.64 && tunel < 0.67) || (tunel > 0.94 && tunel < 0.97))
		{
			int start = noiseList[7]->Octave2D(0.0056 * p_x, 0.0045 * p_y, 3, 0.3) * (hill_height - 8);
			int size = noiseList[5]->Octave2D(0.0126 * p_x, 0.0135 * p_y, 4, 0.5) * 9;
			for (z = start; z < start + size; z++)
			{
				data[pos + z] = AIR;
			}
		}
	return 0;
}

int ChunkGenerator::genWater( int pos, int &z)
{
	if (!data[pos + 60])
				data[pos + 60] = WATER;
	return 0;
}

int ChunkGenerator::gen3DCave(int hill_height, int pos, int &z)
{
	z = 1;
	for ( ; z < hill_height + 2; z++)
	{
		if (z < 32)
		{
			double diamond_factor = noiseList[0]->Octave3D(0.0156 * p_x, 0.095 * p_y, z * 0.39, 1, 0.5);
			if ((diamond_factor > 0.90  || diamond_factor < 0.1)) {
				data[pos + z] = DIAMOUND_BLOCK;
				continue ;
			}
		}
		if (z < 64)
		{
			double iron_factor = noiseList[1]->Octave3D(0.0156 * p_x, 0.0195 * p_y, z * 0.49, 1, 0.5);
			if ((iron_factor > 0.49  && iron_factor < 0.51)) {
				data[pos + z] = IRON_MINERAL;
				continue ;
			}
		}
	}

	z = 1;
	for ( ; z < hill_height + 2; z++)
	{
		double cave_factor = noiseList[3]->Octave3D(0.01556 * p_x, 0.01595 * p_y, z * 0.06, 1, 0.5);
		if (cave_factor > 0.8 || cave_factor < 0.2) {
			data[pos + z] = AIR;
			continue ;
		}
		// double spag_factor = noiseList[0]->Octave3D(0.0256 * p_x, 0.0295 * p_y, z * 0.039, 1, 0.5);
		// if ((spag_factor > 0.41  && spag_factor < 0.44)) {
		// 	data[pos + z] = AIR;
		// 	continue ;
		// }
		
	}
	return 0;
}

#define GEN_NOISE3D false
#define GEN_WATER true

u_char		*ChunkGenerator::generator(Chunk &chunk) {
	sizeX = chunk.sizeX;
	sizeY = chunk.sizeY;
	sizeZ = chunk.sizeZ;

	posX = chunk.GetX();
	posY = chunk.GetY();
	data = (u_char*)calloc(sizeX * sizeY * sizeZ, sizeof(*data));

	
	
	
	engine.seed(389 * posX * posY);

	


	// std::cout << "chunk " << posX << " " << posY << std::endl;

	
	
	for (int y = 0; y < sizeY; y++) {
	for (int x = 0; x < sizeX; x++) {
		ground_height = 0;
		hill_height = 0;
		
		int pos = x * sizeZ + y * sizeX * sizeZ;
		p_x = ((double)posX * sizeX + x);
		p_y = ((double)posY * sizeY + y);
		
		int z = genBedrock(data, x, y);

		genUnderLayer(pos, z);

		genOverLayer(pos, z);


		if (GEN_WATER)
			genWater(pos, z);

		if (GEN_NOISE3D)
			gen3DCave(hill_height, pos, z);

		gen2DCave(hill_height, pos, z);		
		

	}
	}
	if (modifMap.find(position(posX, posY)) == modifMap.end())
		return data;

		
	std::vector<u_char> *modif = modifMap[position(posX, posY)];
	for (int i = 0; i < modif->size(); i+=4)
	{
		data[(*modif)[i] * sizeZ + (*modif)[i + 1] * sizeX * sizeZ + (*modif)[i + 2]] = (*modif)[i + 3];
	}
	
	return data;
}