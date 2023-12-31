#include <classes/World/ChunkRLE.hpp>
#include <map>

		/*****	2 - methods 		*****/
void ChunkRLE::CreateFaceRLE(int type, std::vector<float> &vData, std::vector<u_int> &iData, int x, int y, int z, int offset, int offsetX, int offsetY) {
	
	offset = vData.size() / 3;

	iData.push_back(0 + offset);
    iData.push_back(1 + offset);
    iData.push_back(2 + offset);

    iData.push_back(0 + offset);
    iData.push_back(2 + offset);
    iData.push_back(3 + offset);

	// std::cout << "offset " << offset << " offsetX " << offsetX << " offsetY " << offsetY << std::endl;
	// std::cout << "create face " << type << " x " << x << " y " << y << " z " << z << std::endl;
	// if (posX == 0 && posY == 9 && ((x == 7 && y == 11 ) || (x == 7 && y == 7)  || (x == 8 && y == 7)))
	// {
	// 	std::cout << "not this time" << std::endl;
	// 	return	;
	// }

	// 0: north 1: east 2: south 3: west 4: top 5: bottom
	switch (type)
	{
		case 0 :
		{
			// std::cout << "----north chunk " << posX << " " << posY << " block " << x << " " << y << " " << z << std::endl;
			vData.push_back(0 + x + offsetX); 
			vData.push_back(1 + y + offsetY); 
			vData.push_back(0 + z);

			vData.push_back(0 + x + offsetX); 
			vData.push_back(1 + y + offsetY); 
			vData.push_back(1 + z); 

			vData.push_back(1 + x + offsetX); 
			vData.push_back(1 + y + offsetY); 
			vData.push_back(1 + z); 

			vData.push_back(1 + x + offsetX);
			vData.push_back(1 + y + offsetY);
			vData.push_back(0 + z);
			
			return;
		}
		case 1 :
		{
			vData.push_back(1 + x + offsetX); 
			vData.push_back(0 + y + offsetY); 
			vData.push_back(1 + z);

			vData.push_back(1 + x + offsetX); 
			vData.push_back(0 + y + offsetY); 
			vData.push_back(0 + z); 

			vData.push_back(1 + x + offsetX); 
			vData.push_back(1 + y + offsetY); 
			vData.push_back(0 + z); 

			vData.push_back(1 + x + offsetX);
			vData.push_back(1 + y + offsetY);
			vData.push_back(1 + z);
			return;
		}
		case 2 :
		{
			vData.push_back(0 + x + offsetX); 
			vData.push_back(0 + y + offsetY); 
			vData.push_back(0 + z);

			vData.push_back(1 + x + offsetX); 
			vData.push_back(0 + y + offsetY); 
			vData.push_back(0 + z); 

			vData.push_back(1 + x + offsetX); 
			vData.push_back(0 + y + offsetY); 
			vData.push_back(1 + z); 

			vData.push_back(0 + x + offsetX);
			vData.push_back(0 + y + offsetY);
			vData.push_back(1 + z);
			return;
		}
		case 3 :
		{
			vData.push_back(0 + x + offsetX); 
			vData.push_back(0 + y + offsetY); 
			vData.push_back(1 + z);

			vData.push_back(0 + x + offsetX); 
			vData.push_back(0 + y + offsetY); 
			vData.push_back(0 + z); 

			vData.push_back(0 + x + offsetX); 
			vData.push_back(1 + y + offsetY); 
			vData.push_back(0 + z); 

			vData.push_back(0 + x + offsetX);
			vData.push_back(1 + y + offsetY);
			vData.push_back(1 + z);
			return;
		}
		case 4 :
		{
			vData.push_back(0 + x + offsetX); 
			vData.push_back(0 + y + offsetY);
			vData.push_back(0 + z);

			vData.push_back(0 + x + offsetX); 
			vData.push_back(1 + y + offsetY); 
			vData.push_back(0 + z); 

			vData.push_back(1 + x + offsetX); 
			vData.push_back(1 + y + offsetY); 
			vData.push_back(0 + z); 

			vData.push_back(1 + x + offsetX);
			vData.push_back(0 + y + offsetY);
			vData.push_back(0 + z);
			
			return;
		}
		case 5 :
		{
			vData.push_back(0 + x + offsetX); 
			vData.push_back(0 + y + offsetY);
			vData.push_back(1 + z);

			vData.push_back(0 + x + offsetX); 
			vData.push_back(1 + y + offsetY); 
			vData.push_back(1 + z); 

			vData.push_back(1 + x + offsetX); 
			vData.push_back(1 + y + offsetY); 
			vData.push_back(1 + z); 

			vData.push_back(1 + x + offsetX);
			vData.push_back(0 + y + offsetY);
			vData.push_back(1 + z);

			return;
		}
	}
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
	return false; // Temporary false return
}


u_char* 	ChunkRLE::GetAdjacentRuban(int x, int y, int z, int &pos, u_char direction)
{

	ChunkRLE* 				neighbours;


	switch(direction) //pos du neighbour
	{
		case 0: //north
		{
			if (y == sizeY - 1){	//find north neighbour
				neighbours = GetNeighbour(0);

				if (!neighbours || neighbours->data == NULL)
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
				neighbours = GetNeighbour(1);
				if (!neighbours || neighbours->data == NULL)
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
				neighbours = GetNeighbour(2);
				if (!neighbours || neighbours->data == NULL)
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
				neighbours = GetNeighbour(3);
				if (!neighbours || neighbours->data == NULL)
					return (NULL);
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



void	ChunkRLE::CompileData()
{
	
	int nbr_points = 0; // pas bien
	int pos = 0;
	for (int y = 0; y < sizeY; y++) {
	for (int x = 0; x < sizeX; x++) {
		// std::cout << "----block x " << x << " y " << y << std::endl;
		//pour chaque X-Y
		int z = 0;

		int neighb_pos[4] = {0, 0, 0, 0};
		int neighb_z[4] = {0, 0, 0, 0};
		int neighb_over[4] = {0, 0, 0, 0};
		while (z < (sizeZ - 1)) //sur toute la hauteure
		{
			int z_max = z + data[pos + 1];
			if (!z || (data[pos] && !data[pos - 2]))
			{	//create bottom face
				CreateFaceRLE(4, vertexData, shapeAssemblyData, x, y, z, vertexData.size(), posX * sizeX, posY * sizeY);
				nbr_points += 4;
			}
			for (u_char neighb = 0; neighb < 4; neighb++) // pour chaque voisin (gerer si deja over)
			{
				u_char* ruban = this->GetAdjacentRuban(x, y, z, neighb_pos[neighb], neighb);

				int END = z_max;
				
				while (neighb_z[neighb] < z_max) //tant que le voisin n'est pas plus haut que la fin du ruban
				{
					if (this->data[pos] == 0 || (ruban && ruban[neighb_pos[neighb]] != 0))
					{
						incrementNeighb(neighb_pos[neighb], neighb_z[neighb], z_max - z, z_max, neighb_over[neighb]);
						continue ;
					}

					int neighb_size;
					if (ruban)
						neighb_size = ruban[neighb_pos[neighb] + 1];
					else // if no neighbour we draw all the ruban anyway
						neighb_size = sizeZ - z;

					if (neighb_over[neighb]) // need to inspect more this
						neighb_size =  neighb_over[neighb];

					if (neighb_z[neighb] + neighb_size <= z_max) 
						END = neighb_z[neighb] + neighb_size;

					for (int i = neighb_z[neighb]; i < END     ; i++)
					{	//create side faces
						// if (posX == 0 && x >= sizeX - 5)
						// 	continue;
						CreateFaceRLE(neighb, vertexData, shapeAssemblyData, x, y, i, vertexData.size(), posX * sizeX, posY * sizeY);
						nbr_points += 4;
					}
					if (!ruban)
						incrementNeighb(neighb_pos[neighb], neighb_z[neighb], END - neighb_z[neighb], z_max, neighb_over[neighb]);
					else
						incrementNeighb(neighb_pos[neighb], neighb_z[neighb], ruban[neighb_pos[neighb] + 1], z_max, neighb_over[neighb]);
					
				}
			}
			
			if (data[pos] && z < sizeZ - 1 && !data[pos + 2]) 
			{	//create top face
				CreateFaceRLE(5, vertexData, shapeAssemblyData, x, y, z_max-1, vertexData.size(), posX * sizeX, posY * sizeY);
				nbr_points += 4;
			}
			pos += 2;
			z = z_max;
		}
	}
	}

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
	int ret = this->rubansIndexes[x][y];
	int pos = 0;
	// std::cout << "x = " << x << " y = " << y << " z = " << z << std::endl;
	// std::cout << "ret = " << ret << std::endl;
	// std::cout << "sizeData = " << sizeData << std::endl;
	while (pos <= z && ret < sizeData)
	{
		// std::cout << "in ret = " << ret << std::endl;
		pos += this->data[ret + 1];
		ret += 2;
	}
	if (ret >= sizeData)
	{
		// std::cout << "OVERFLOW " << x << " " << y << " " << z  << std::endl;
		ret = sizeData;
	}
	// std::cout << "ruban pos " << pos << std::endl;
	// std::cout << "ruban ret " << ret << std::endl;
	return (ret - 2);
}

void 					ChunkRLE::Generate()
{

	data = (u_char*)malloc(sizeof(u_char) * sizeX * sizeY * 4);

	int max_z = 200;
	for (int pos = 0; pos < sizeX * sizeY * 4; pos+=4)
	{
		// if (pos == 25*4)
		// 	max_z++;
		rubansIndexes[pos / 4 % sizeX ][pos / 4 / sizeX] = pos;
		this->rubans_id[pos/4] = pos;
		if (pos == 25*4)
			max_z++;
		data[pos] = 12;
		data[pos + 1] = max_z;
		data[pos + 2] = 0;
		data[pos + 3] = (sizeZ - 1) - max_z;
		if (pos == 25*4)
			max_z--;
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