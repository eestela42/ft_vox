#include <classes/World/ChunkRLE.hpp>
#include <map>


u_char* 	ChunkRLE::GetAdjacentRuban(int &pos, u_char direction)
{
	switch(direction)
	{
		case 0: //north
			pos += sizeX * 2;
		case 1: //east
			pos += 2;
		case 2: //south
			pos -= sizeX * 2;
		case 3: //west
			pos -= 2;
	}

	if (direction == 1 && pos % sizeX == sizeX - 1){	//find east neighbour
		if (!this->_neighbours[1])
			return (NULL);
		pos = calcY(pos) * sizeX * 2;
	}
	else if (direction == 3 && pos % sizeX == 0){	//find west neighbour
		if (!this->_neighbours[3])
			return (NULL);
		pos = ((calcY(pos) + 1) * sizeX - 1) * 2;
	}

	else if (direction == 0 && pos >= sizeX * sizeY){	//find north neighbour
		if (!this->_neighbours[0])
			return (NULL);
		pos = calcX(pos) * 2;
	}
	else if (direction == 2 && pos / sizeX == 0){	//find south neighbour
		if (!this->_neighbours[2])
			return (NULL);
		pos = (sizeX * (sizeY - 1) + calcX(pos)) * 2;
	}
	return (this->data);
}



void	incrementNeighb(int &pos, int &z, int incr, int z_max, int &over)
{
	//over = pos de end du voisin != z + incr , mais est == z + over
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
	else if (over && z + over > z_max) // si deja over et toujours over
	{
		over -= z_max - z;
		z = z_max;
	}
	else if (over && z + over <= z_max) // si etait over mais ne va plus l'etre
	{
		over = 0;
		z += incr - over;
		pos += 2;
	}
}

void	ChunkRLE::DataCompiler()
{
	int x, y, z;
	for (int pos = 0; pos < sizeX * sizeY * 2; pos+=2) // tant que dans le RLE
	{
		//pour chaque X-Y
		x = pos/2 % sizeX;
		y = pos/2 / sizeX;
		z = 0;

		int neighb_pos[4] = {0, 0, 0, 0};
		int neighb_z[4] = {0, 0, 0, 0};
		int neighb_over[4] = {0, 0, 0, 0};

		while (z != sizeZ) //sur toute la hauteure
		{
			int z_max = z + data[pos + 1];
			for (u_char neighb = 0; neighb < 4; neighb++) // pour chaque voisin (gerer si deja over)
			{
				u_char* ruban = this->GetAdjacentRuban(pos, neighb);
				int END = z_max;
				
				while (neighb_z[neighb] < z_max) //tant que le voisin n'est pas plus haut que la fin du ruban
				{
					if (this->data[pos] == 0 || (ruban && ruban[pos] != 0))
					{
						incrementNeighb(neighb_pos[neighb], neighb_z[neighb], ruban[pos + 1], z_max, neighb_over[neighb]);
						continue ;
					}
					int neigh_size = ruban[pos + 1];

					if ( neighb_over[neighb])
						neigh_size =  neighb_over[neighb];

					if (neighb_z[neighb] + neigh_size <= z_max)
						END = neighb_z[neighb] + neigh_size;
					
					// create face from neighb to END
					incrementNeighb(neighb_pos[neighb], neighb_z[neighb], ruban[pos + 1], z_max, neighb_over[neighb]);
				}
			}
		}
	}
}

		/*****	1 - constructors 		*****/

ChunkRLE::~ChunkRLE()
{
}

ChunkRLE::ChunkRLE() : Chunk()
{
	this->data = NULL;
}

std::vector<u_char>&	ChunkRLE::GetVertexData()
{

}

std::vector<u_int> &	ChunkRLE::GetShapeAssemblyData()
{

}

int ChunkRLE::calcX(int pos)
{
	return ((pos/2) % this->sizeX);
}

int ChunkRLE::calcY(int pos)
{
	return ((pos/2) / this->sizeX);
}

void 					ChunkRLE::Generate()
{
	data = (u_char*)malloc(sizeof(u_char) * sizeX * sizeY * 4);

	int max_z = 10;
	for (int pos = 0; pos < sizeX * sizeY * 4; pos+=4)
	{
		data[pos] = 1;
		data[pos + 1] = max_z;
		data[pos + 2] = 0;
		data[pos + 3] = (sizeZ - 1) - max_z;
	}
}

void 					ChunkRLE::Generate(std::vector<glm::ivec3> positionList,
										std::vector<glm::ivec3> sizeList)
{
	std::vector<u_char> rubans;
	rubans.resize(sizeX * sizeY * 2);
	for (int x = 0; x < sizeX; x++)
	{
		for (int y = 0; y < sizeY; y++)
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



