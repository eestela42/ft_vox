#include <classes/World/ChunkRLE.hpp>
#include <map>
#include <classes/World/PerlinNoise.hpp>
#include <iostream>
#include <cstring>

		/*****	2 - methods 		*****/
void ChunkRLE::createPointVertex(std::vector<int> &vertexes, int pos, u_char orientation, u_char type)
{
	for (char i = 0; i < 4; i++)
	{
		u_int data[3];
		for (int i = 0; i < 3; i++)
			data[i] = 0;

		data[0] = this->posX << 8;
		data[0] += (this->posY >> 16) & 0x000000FF;
		data[1] = (this->posY & 0x0000FFFF) << 16;
		data[1] += (pos >> 2) & 0x0000FFFF;
		int tmp = 0;
		tmp = (orientation << 8) + type;
		data[2] = tmp << 14;
		data[2] += pos << 30;
		//faire les casts

		float outData[3];
		// std::cout << "data[0] : " << data[0] << std::endl;

		vertexes.push_back(data[0]);
		vertexes.push_back(data[1]);
		vertexes.push_back(data[2]);

		int data1 = data[0];
		int data2 = data[1];
		int data3 = data[2];

		int chunk_x = 0;
		int chunk_y = 0;
		int pos = 0;
		int face = 0;
		int type = 0;
		int point = 0;
		chunk_x = data1 >> 8;
		chunk_y = (data1 << 16) & 0x00FF0000;
		chunk_y = (data2 >> 16) & 0x0000FFFF; 
		pos = (data2 & 0x0000FFFF) << 2;
		pos += (data3 & 0xC0000000) >> 30;
		face = (data3 >> 22) & 0x000000FF;
		type = (data3 >> 14) & 0x000000FF;


		// std::cout << "data[0] : " << data[0] << std::endl;
	}
}

void ChunkRLE::CreateFaceRLE(int orientation, std::vector<int> &vData, std::vector<u_int> &iData, int x, int y, int z, int offset, u_char type) {
	
	offset = vData.size() / 3;

	iData.push_back(0 + offset);
    iData.push_back(1 + offset);
    iData.push_back(2 + offset);

    iData.push_back(0 + offset);
    iData.push_back(2 + offset);
    iData.push_back(3 + offset);

	// 0: north 1: east 2: south 3: west 4: top 5: bottom


	int pos = x + y * this->sizeX + z * this->sizeX * this->sizeY;
	int vx = pos % sizeX;
	int vy = (pos % (sizeX * sizeY)) / sizeY;
	int vz = pos / (sizeX * sizeY);

	createPointVertex(vData, pos, orientation, type);

	
}

/* 
 * Here you need to implement a method that checks whether a specific point in the chunk is filled so neighbors are able to render properly
 * Because neighbors might not be the same Chunk type as you, every chunk has to implement this since it is a virtual method
 * I know it doesn't fit RLE much, but you could optimize this with static variables, remembering where you left off last call
*/
bool ChunkRLE::isFilled(int x, int y, int z) {
	if (!IsGenerated()) { //This is supposed to be impossible right now WHATEVER you do, but it costs nothing
		return false;
	}
	int pos = this->rubansIndexes[x][y];
	int tmp_z = 0;
	while (tmp_z <= z)
	{
		tmp_z += this->data[pos + 1];
		pos += 2;
	}
	if (this->data[pos - 2] != 0)
		return true;
	return false;
}


u_char* 	ChunkRLE::GetAdjacentRuban(int x, int y, int z, int &pos, u_char direction)
{

	ChunkRLE* 				neighbours;


	switch(direction) //pos du neighbour
	{
		case 0: //north
		{
			if (y == sizeY - 1){	//find north neighbour
				neighbours = (ChunkRLE*)(neighborChunks[0]);

				if (neighbours->posY != this->posY + 1  && neighbours->posX == this->posX)
					return (NULL);
				pos = neighbours->GetRubanPos(x, 0, z);
				return (neighbours->data);
			}
			pos = GetRubanPos(x, y + 1, z);
			break;
		}
		case 1: //east
		{
			if (x == sizeX - 1) //find east neighbour
			{	
				neighbours = (ChunkRLE*)(neighborChunks[1]);
				if (neighbours->posX != this->posX + 1  && neighbours->posY == this->posY)
					return (NULL);
				pos = neighbours->GetRubanPos(0, y, z);
				return (neighbours->data);
			}
			pos = GetRubanPos(x+1, y, z);
			break;
		}
		case 2: //south
		{
			if (y == 0)
			{	//find south neighbour
				neighbours = (ChunkRLE*)(neighborChunks[2]);
				if (neighbours->posY != this->posY - 1  && neighbours->posX == this->posX)
					return (NULL);
				pos = neighbours->GetRubanPos(x, sizeY - 1, z);
				return (neighbours->data);
			}
			pos = GetRubanPos(x, y-1, z);
			break;
		}
		case 3: //west
		{
			if (x == 0)//find west neighbour
			{
				neighbours = (ChunkRLE*)(neighborChunks[3]);
				if (neighbours->posX != this->posX - 1 && neighbours->posY == this->posY)
				{
					return (NULL);
				}
				pos = neighbours->GetRubanPos(sizeX - 1, y, z);
				return (neighbours->data);
			}
			pos = GetRubanPos(x-1, y, z);
			break;
		}
	}

	return (this->data);
}

void	incrementNeighb(int &pos, int &z, int incr, int z_max, int &over)
{
	if (!over && z + incr <= z_max) // si tout va bien
	{
		z += incr;
		pos += 2;
	}
	else if (!over && z + incr > z_max) // si next voisin > z_max
	{
		over = (z + incr) - z_max;
		z = z_max;
	}
	else if (over && z + over >= z_max) // si deja over et toujours over
	{
		over -= z_max - z;
		z = z_max;
	}
	else if (over && z + over < z_max) // si etait over mais ne va plus l'etre
	{
		over = 0;
		z += incr - over;
		pos += 2;
	}

}



void	ChunkRLE::CompileData()
{
	int nbr_points = 0; // pas bien
	int pos = 0;
	for (int y = 0; y < sizeY; y++) {
	for (int x = 0; x < sizeX; x++) {
		//pour chaque X-Y
		int z = 0;

		int neighb_pos[4] = {0, 0, 0, 0};
		int neighb_z[4] = {0, 0, 0, 0};
		int neighb_over[4] = {0, 0, 0, 0};
		while (z < (sizeZ - 1)) //sur toute la hauteure
		{
			int z_end = z + data[pos + 1];
			if (!z || (data[pos] && !data[pos]))
			{	//create bottom face
				CreateFaceRLE(4, vertexData, shapeAssemblyData, x, y, z, vertexData.size(), data[pos]);
				nbr_points += 4;
			}
			for (u_char neighb = 0; neighb < 4; neighb++) // pour chaque voisin (gerer si deja over)
			{
				u_char* ruban = this->GetAdjacentRuban(x, y, z, neighb_pos[neighb], neighb);
				int count = 0;
				while (neighb_z[neighb] < z_end) //tant que le voisin n'est pas plus haut que la fin du ruban
				{
					if (this->data[pos] == 0)
					{
						incrementNeighb(neighb_pos[neighb], neighb_z[neighb], z_end - z, z_end, neighb_over[neighb]);
						continue ;
					}
					if (ruban && ruban[neighb_pos[neighb]] != 0)
					{
						incrementNeighb(neighb_pos[neighb], neighb_z[neighb], ruban[neighb_pos[neighb] + 1], z_end, neighb_over[neighb]);
						continue ;
					}
					int to_draw = 0;
					int neighb_size = 0;
					if (!ruban)
						to_draw = z_end - z;
					if (ruban)
					{
						neighb_size = ruban[neighb_pos[neighb] + 1];
						
						if (neighb_over[neighb]) // really ?
							neighb_size =  neighb_over[neighb];

						if (neighb_z[neighb] + neighb_size <= z_end) 
							to_draw = neighb_size;
						else
							to_draw = z_end - neighb_z[neighb];
						
					}
					int i = neighb_z[neighb];

					if (!ruban)
						incrementNeighb(neighb_pos[neighb], neighb_z[neighb], to_draw, z_end, neighb_over[neighb]);
					else
						incrementNeighb(neighb_pos[neighb], neighb_z[neighb], ruban[neighb_pos[neighb] + 1], z_end, neighb_over[neighb]);


					for (; to_draw--; i++)
					{	//create side faces
						CreateFaceRLE(neighb, vertexData, shapeAssemblyData, x, y, i, vertexData.size(), data[pos]);
						nbr_points += 4;
					}
					
					//end while
				}
			}
			if (data[pos] && z < sizeZ - 1 && !data[pos + 2]) 
			{	//create top face
				CreateFaceRLE(5, vertexData, shapeAssemblyData, x, y, z_end-1, vertexData.size(), data[pos]);
				nbr_points += 4;
			}
			pos += 2;
			z = z_end;
		}
	}
	}
			std::cout << " ----in compile data \n";

	for (int i = 0; i < 5; i++)
	{
		std::cout << vertexData[i] << " ";
	}
	std::cout << std::endl;
}

		/*****	1 - constructors 		*****/

ChunkRLE*	ChunkRLE::GetNeighbour(int cardinal)
{
	ChunkRLE*	neighbour = NULL;

	switch (cardinal)
	{
	case 0:
		neighbour = (ChunkRLE*)loadedChunks[posX % loadedChunks.size()][(posY + 1) % loadedChunks.size()];
		if (neighbour && neighbour->posY == this->posY + 1)
		{
			return (neighbour);
		}
		return (NULL);
	case 1:
		neighbour = (ChunkRLE*)loadedChunks[(posX + 1) % loadedChunks.size()][posY % loadedChunks.size()];
		if (neighbour && neighbour->posX == this->posX + 1)
		{
			return (neighbour);
		}
		return (NULL);
	case 2:
		neighbour = (ChunkRLE*)loadedChunks[posX % loadedChunks.size()][(posY - 1) % loadedChunks.size()];
		if (neighbour && neighbour->posY == this->posY - 1)
		{
			return (neighbour);
		}
		return (NULL);
	case 3:
		neighbour = (ChunkRLE*)loadedChunks[(posX - 1) % loadedChunks.size()][posY % loadedChunks.size()];
		if (neighbour && neighbour->posX == this->posX - 1)
		{
			return (neighbour);
		}
		return (NULL);
	}
	
	return (NULL);
	
}

ChunkRLE::~ChunkRLE()
{
	if (this->data)
		delete this->data;
	delete this->rubans_id;
	
}

ChunkRLE::ChunkRLE() : Chunk(0,0)
{
	this->data = NULL;
	this->rubans_id = new u_int[256];
}

ChunkRLE::ChunkRLE(int posX, int posY) : Chunk(posX, posY)
{
	this->data = NULL;
	this->rubans_id = new u_int[256];
}


int ChunkRLE::calcX(int pos)
{
	return ((pos/2) % this->sizeX);
}

int ChunkRLE::calcY(int pos)
{
	return ((pos/2) / this->sizeX);
}

u_int					ChunkRLE::GetRubanPos(int x, int y, int z)
{
	int pos = this->rubansIndexes[x][y];
	int tmp_z = 0;

	while (tmp_z <= z )
	{
		tmp_z += this->data[pos + 1];
		pos += 2;
	}
	
	return (pos - 2);
}


void 					ChunkRLE::Generate()
{
	u_int seed = 988456;

	PerlinNoise addrNoise(seed);
	PerlinNoise *noise = &addrNoise;

	PerlinNoise addrNoise2(seed + 13);
	PerlinNoise *noise2 = &addrNoise2;

	PerlinNoise addrNoise3(seed + 17);
	PerlinNoise *noise3 = &addrNoise3;


	data = (u_char*)malloc(sizeof(u_char) * sizeX * sizeY * 4);

	int max_z = 5;
	int pos = 0;

	int x_tab = 0;
	int y_tab = 0;
	for (int y = 0; y < sizeY; y++){
	for (int x = 0; x < sizeX; x++)
	{
		rubansIndexes[x][y] = pos;
		this->rubans_id[pos/4] = pos;
		double p_x = (double)((posX * sizeX + x))/((double)(sizeX * 7));
		double p_y = (double)((posY * sizeY + y))/((double)(sizeY * 7));


		u_char outPut =  5 + (int)(noise->newNoise2d(10 * p_x, 10 * p_y, 0.8) * 15)
							+ (int)(noise3->newNoise2d(9 * p_x, 9 * p_y, 0.65) * 14)
							+ (int)(noise2->newNoise2d(9 * p_x, 9 * p_y, 0.65) * 16);

		data[pos] = 12;
		data[pos + 1] = outPut % 255;
		data[pos + 2] = 0;
		data[pos + 3] = (sizeZ - 1) -  outPut % 255;
		
		pos += 4;
	}
	}
	this->sizeData = sizeX * sizeY * 4;
	
}

void 					ChunkRLE::GenerateTest(PerlinNoise *noise, PerlinNoise *noise2)
{
	u_int seed = 988456;

	data = (u_char*)malloc(sizeof(u_char) * sizeX * sizeY * 4);

	int max_z = 5;
	int pos = 0;

	int x_tab = 0;
	int y_tab = 0;
	for (int y = 0; y < sizeY; y++){
	for (int x = 0; x < sizeX; x++)
	{
		rubansIndexes[x][y] = pos;
		this->rubans_id[pos/4] = pos;
		double p_x = (double)((posX * sizeX + x))/((double)(sizeX * 12));
		double p_y = (double)((posY * sizeY + y))/((double)(sizeY * 12));


		u_char outPut =  5 + (int)(noise->newNoise2d(10 * p_x, 10 * p_y, 0.8) * 8)
							// + (int)(noise3->newNoise2d(9 * p_x, 9 * p_y, 0.65) * 14)
							+ (int)(noise2->newNoise2d(5 * p_x, 5 * p_y, 0.65) * 9);

		data[pos] = pos % 255;
		data[pos + 1] = outPut % 255;
		data[pos + 2] = 0;
		data[pos + 3] = (sizeZ - 1) -  outPut % 255;
		
		pos += 4;
	}
	}
	this->sizeData = sizeX * sizeY * 4;
	
}


void 					ChunkRLE::Generate(std::vector<glm::ivec3> positionList,
										std::vector<glm::ivec3> sizeList)
{
	std::vector<u_char> rubans;
	rubans.resize(sizeX * sizeY * 2);
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			std::vector<u_char> objRubans;
			for (int obj = 0; obj < positionList.size(); obj++) // trouver tous les rubans en cette pos
			{
				if (x >= positionList[obj].x && x < positionList[obj].x + sizeList[obj].x &&
					y >= positionList[obj].y && y < positionList[obj].y + sizeList[obj].y)
				{
						objRubans.push_back(obj % (sizeZ - 1) + 1);
						objRubans.push_back(positionList[obj].z);
						objRubans.push_back(sizeList[obj].z);
				}
			}
			this->rubans_id[x * sizeY + y] = rubans.size();
			int z = 0;
			int filled = 0;
			while (objRubans.size())
			{
				int min = 0;
				int indexObj = 0;
				for (int i = 1; i < objRubans.size(); i+=3)
					if (objRubans[i] < objRubans[min]) // find the lowest block
					{
						min = objRubans[i];
						indexObj = i;
					}
				if (min > filled) // si air avant
				{
					rubans.push_back(0);
					rubans.push_back(min - filled);
					rubans.push_back(objRubans[indexObj - 1]);
					rubans.push_back(objRubans[indexObj + 1]);
					z = objRubans[indexObj];
					filled = objRubans[indexObj] + objRubans[indexObj + 1];
				}
				else if (min <= filled && min + objRubans[indexObj + 1] > filled)
				{
					rubans.push_back(objRubans[indexObj - 1]);
					rubans.push_back(objRubans[indexObj] + objRubans[indexObj + 1] - filled);
					z = objRubans[indexObj];
					filled = objRubans[indexObj] + objRubans[indexObj + 1];
				}
				
				objRubans.erase(objRubans.begin() + indexObj - 1, objRubans.begin() + indexObj + 1);
					
			}
			if (filled < sizeZ)
			{
				rubans.push_back(0);
				rubans.push_back(255 - filled);
			}
		}
	}
}

void 					ChunkRLE::Generate(u_int seed)
{}