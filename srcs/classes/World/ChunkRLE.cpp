#include <classes/World/ChunkRLE.hpp>
#include <map>
#include <classes/World/PerlinNoise.hpp>
#include <iostream>
#include <cstring>


std::vector<PerlinNoise*> 		ChunkRLE::noiseList;
std::vector<std::vector<float>> ChunkRLE::weightList;

		/*****	2 - methods 		*****/
void ChunkRLE::createPointVertex(std::vector<int> &vertexes, int pos, u_char orientation, u_char type)
{
	for (char i = 0; i < 4; i++)
	{
		vertexes.push_back(posX);
		vertexes.push_back(posY);
		vertexes.push_back(pos);
		vertexes.push_back(orientation);
		vertexes.push_back(type);
		if (orientation == 2 || orientation == 3 || orientation == 4)
			vertexes.push_back(8);
		else
			vertexes.push_back(10);
	}
}

void ChunkRLE::CreateFaceRLE(int orientation, std::vector<int> &vData, std::vector<u_int> &iData, int x, int y, int z, int offset, u_char type) {
	
	offset = vData.size() / 6;

	iData.push_back(0 + offset);
    iData.push_back(1 + offset);
    iData.push_back(2 + offset);

    iData.push_back(0 + offset);
    iData.push_back(2 + offset);
    iData.push_back(3 + offset);

	int pos = x + y * this->sizeX + z * this->sizeX * this->sizeY;

	createPointVertex(vData, pos, orientation, type);

	
}


bool ChunkRLE::isFilled(int x, int y, int z) {
	if (!IsGenerated()) { 
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

				if (!neighbours || (neighbours->posY != this->posY + 1  && neighbours->posX == this->posX))
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
				if (!neighbours || (neighbours->posX != this->posX + 1  && neighbours->posY == this->posY))
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
				if (!neighbours || (neighbours->posY != this->posY - 1  && neighbours->posX == this->posX))
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
				if (!neighbours || (neighbours->posX != this->posX - 1 && neighbours->posY == this->posY))
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

void	incrementNeighb(int& neighb_pos, int& neighb_z, int& incr, int neighb_size, int& over)
{
	if (over + incr >= neighb_size)
	{
		neighb_pos += 2;
		neighb_z += neighb_size;
		incr -= neighb_size - over;
		over = 0;
		return ;
	}
	over += incr;
	incr = 0;
}

void	ChunkRLE::CompileData()
{
	// std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	vertexData.clear();
	shapeAssemblyData.clear();
	vertexData.reserve(20000);
	shapeAssemblyData.reserve(7000);

	u_int pos = 0;

	for (u_int y = 0; y < sizeY; y++) {
	for (u_int x = 0; x < sizeX; x++)
	{
		u_int z = 0;
		int neighb_pos[4] = {0, 0, 0, 0}; 	//pos in neighbour's rubans
		int neighb_z[4] = {0, 0, 0, 0};		//z postion of the neighbour
		int neighb_over[4] = {0, 0, 0, 0};	//nbr of block already checked in this ruban 
		while (z < sizeZ - 1)
		{
			u_int z_end  = z + data[pos + 1];
			if ((!z && data[pos]) || (data[pos] && !data[pos - 2]))
				CreateFaceRLE(4, vertexData, shapeAssemblyData, x, y, z, vertexData.size(), data[pos]);
			for (u_int neighb = 0; neighb < 4; neighb++)
			{
				u_char *ruban = GetAdjacentRuban(x, y , z, neighb_pos[neighb], neighb);
				while (neighb_z[neighb] + neighb_over[neighb] < z_end)
				{

					int real_z = neighb_z[neighb] + neighb_over[neighb];
					int neighb_size = data[pos + 1];
					int to_draw = z_end - z;
					int tmp_pos = x + y * this->sizeX + z * this->sizeX * this->sizeY;
					if (ruban)
						neighb_size = ruban[neighb_pos[neighb] + 1];
					if (data[pos] == 0) 
					{
						if (ruban)
						{
							int real_size = ruban[neighb_pos[neighb] + 1] - neighb_over[neighb];
							if (real_z + real_size < z_end)
								to_draw = real_size;
							else
								to_draw = real_size - (real_z + real_size - z_end);
						}
						incrementNeighb(neighb_pos[neighb], neighb_z[neighb], to_draw, neighb_size, neighb_over[neighb]);
						continue ;
					}

					if (ruban && ruban[neighb_pos[neighb]] != 0)
					{
						int real_size = ruban[neighb_pos[neighb] + 1] - neighb_over[neighb];
						if (real_z + real_size < z_end)
							to_draw = real_size;
						else
							to_draw = real_size - (real_z + real_size - z_end);
						incrementNeighb(neighb_pos[neighb], neighb_z[neighb], to_draw, neighb_size, neighb_over[neighb]);
						continue ;
					}
					if (ruban)
					{
						int real_size = ruban[neighb_pos[neighb] + 1] - neighb_over[neighb];
						to_draw = 0;
						if (real_z + real_size <= z_end)
							to_draw = real_size;
						else
							to_draw = real_size - (real_z + real_size - z_end);
					}
					for (int i = 0; to_draw - i; i++)
						CreateFaceRLE(neighb, vertexData, shapeAssemblyData, x, y, real_z + i, vertexData.size(), data[pos]);
					while (to_draw)
					{
						incrementNeighb(neighb_pos[neighb], neighb_z[neighb], to_draw, neighb_size, neighb_over[neighb]);
						if (ruban)
							neighb_size = ruban[neighb_pos[neighb] + 1];
					}
				}
			}
			if (data[pos] && z < sizeZ - 1 && !data[pos + 2]) 
				CreateFaceRLE(5, vertexData, shapeAssemblyData, x, y, z_end-1, vertexData.size(), data[pos]);
			pos += 2;
			z = z_end;
		}
	}
	}
	dataStruct.data = (u_char*)vertexData.data();
	dataStruct.size = vertexData.size() * sizeof(int);
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

void ChunkRLE::pushBackNoiseList(PerlinNoise* tmp)
{
	ChunkRLE::noiseList.push_back(tmp);
}

void ChunkRLE::pushBackWeightList(std::vector<float> tmp)
{
	ChunkRLE::weightList.push_back(tmp);
}

void ChunkRLE::randomGen(int &pos, int x, int y)
{
	
		double p_x = ((double)posX * sizeX + x);
		double p_y = ((double)posY * sizeY + y);

		// p_x += 0.1;
		// p_y += 0.1;

		p_x /= (double)(sizeX * 16);
		p_y /= (double)(sizeY * 16);

		u_char outPut = 0;
		double desert = noiseList[2]->Octave2D(0.00556 * p_x, 0.00645 * p_y, 4, 1.53);

		int ground_factor = 100 ; 	//rock
		int hill_factor = 50 * desert;	// over layer

		u_char type_under = DIRT;
		u_char type_over = GRASS;
		if (desert < 0.5)
		{
			type_under = SAND;
			type_over = SAND;
		}

		double v1 = noiseList[0]->Octave2D(0.0456 * p_x, 0.0395 * p_y, 4, 5);

		outPut = (int)(v1 * ground_factor);
		data[pos + 0] = STONE;
		data[pos + 1] = outPut % 255 ;
		pos += 2;


		


		double v2 = (noiseList[1]->Octave2D(0.554 * p_x, 0.454 * p_y, 5, 0.1 + (0.5 / desert / v1)));
		outPut = (int)(v2 * hill_factor * (0.5 + v1));

		data[pos + 0] = type_under;
		data[pos + 1] = outPut % 255 ;
		pos += 2;

		// if ((data[pos-1] + data[pos-3] > 60))
		// 	data[pos + 0] = GRASS;
		// else
		data[pos + 0] = type_over;
		data[pos + 1] = 1;
		pos += 2;
}

void 					ChunkRLE::Generate()
{
	data = (u_char*)malloc(sizeof(u_char) * sizeX * sizeY * 20);

	int pos = 0;

	int x_tab = 0;
	int y_tab = 0;



	for (int y = 0; y < sizeY; y++){
	for (int x = 0; x < sizeX; x++)
	{
		rubansIndexes[x][y] = pos;

		///bot
		data[pos + 0] = BEDROCK;
		data[pos + 1] = 3;
		pos += 2;


		randomGen(pos, x, y);
		

		//top
		data[pos + 0] = 0;
		data[pos + 1] = (u_char)sizeZ - 4;
		
		pos += 2;

	}
	}
	this->sizeData = sizeX * sizeY * 10;
	
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

