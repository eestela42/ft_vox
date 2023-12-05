#include <classes/World/Chunk.hpp>


		/*****	1 - constructors 		*****/

Chunk::~Chunk()
{
	delete [] this->data;
}

Chunk::Chunk() : pos_x(0), pos_y(0)
{
	this->data = new u_char[this->size_x * this->size_y * this->size_z];
	for (int i = 0; i < this->size_x * this->size_y * this->size_z; i++)
		this->data[i] = 0;
	rle.CreateFromData(this->data);
}

Chunk::Chunk(int x, int y) : Chunk()
{
	pos_x = x;
	pos_y = y;
}

Chunk::Chunk(int x, int y, RLE &other) : Chunk()
{
	pos_x = x;
	pos_y = y;
	this->rle = other;
}

Chunk::Chunk(int x, int y, float seed): Chunk()
{
	pos_x = x;
	pos_y = y;
	this->rle = RLE(pos_x, pos_y, seed);
}


		/*****	2 - fillers	 	*****/



		/*****	3 - toVbo		 	*****/

void Chunk::CreatePointVertex(std::vector<int> &vertexes, int pos, u_char orientation, u_char type)
{
	for (char i = 0; i < 4; i++)
	{
		u_int data[3];
		for (int i = 0; i < 3; i++)
			data[i] = 0;

		data[0] = this->pos_x << 8;
		data[0] += (this->pos_y >> 16) & 0x000000FF;
		data[1] = (this->pos_y & 0x0000FFFF) << 16;
		data[1] += (pos >> 2) & 0x0000FFFF;
		int tmp = 0;
		tmp = (orientation << 8) + type;
		data[2] = tmp << 14;
		data[2] += pos << 30;
		vertexes.push_back(data[0]);
		vertexes.push_back(data[1]);
		vertexes.push_back(data[2]);
	}
}

void Chunk::CreateTrianglesFace(int start, std::vector<unsigned int> &triangles)
{
	triangles.push_back(start + 0);
	triangles.push_back(start + 1);
	triangles.push_back(start + 2);


	triangles.push_back(start + 0);
	triangles.push_back(start + 2);
	triangles.push_back(start + 3);
}

void Chunk::Parkour(int start_vert, std::vector<int> &vertexes,std::vector<unsigned int> &triangles, bool *tab, int pos)
{
	tab[pos] = true;
	int x = pos % this->size_x;
	int y = (pos / this->size_x) % this->size_y;
	int z = pos / (this->size_x * this->size_y);
	if ( x == size_x - 1 || data[pos + 1] == 0)
	{
		CreateTrianglesFace(start_vert + vertexes.size() / size_vertex, triangles);

		CreatePointVertex(vertexes, pos, 0, data[pos]);
	}
	if (x == 0 || data[pos - 1] == 0)
	{
		CreateTrianglesFace(start_vert + vertexes.size() / size_vertex, triangles);

		CreatePointVertex(vertexes, pos, 1, data[pos]);

	}
	if (y == size_y - 1 || data[pos + this->size_x] == 0)
	{
		CreateTrianglesFace(start_vert + vertexes.size() / size_vertex, triangles);

		CreatePointVertex(vertexes, pos, 2, data[pos]);
	}
	if (y == 0 || data[pos - this->size_x] == 0)
	{
		CreateTrianglesFace(start_vert + vertexes.size() / size_vertex, triangles);

		CreatePointVertex(vertexes, pos, 3, data[pos]);
	}
	if (z == size_z - 1 || data[pos + this->size_x * this->size_y] == 0)
	{
		CreateTrianglesFace(start_vert + vertexes.size() / size_vertex, triangles);

		CreatePointVertex(vertexes, pos, 4, data[pos]);
	}
	if (z == 0 || data[pos - this->size_x * this->size_y] == 0)
	{
		CreateTrianglesFace(start_vert + vertexes.size() / size_vertex, triangles);

		CreatePointVertex(vertexes, pos, 5, data[pos]);
	}
	return;
}



void	Chunk::RleToVbo(std::vector<int> &out_vertexes, std::vector<unsigned int> &out_triangles)
{
	bool tab[this->size_x*this->size_y*this->size_z];
	for (int i = 0; i < this->size_x*this->size_y*this->size_z; i++)
		tab[i] = false;

	int XY = 0;
	for (std::vector<std::vector<u_char>>::iterator ruban = this->rle.GetRubans().begin(); ruban != this->rle.GetRubans().end(); ruban++)
	{
		int zero = 0;
		for (int i = 0; i < (*ruban).size(); i += 2)
		{
			int pos = XY % size_x + (XY / size_x) * size_y;
			if ((*ruban)[i] == 0)
				continue;
			for (int z = 0; z < (*ruban)[i + 1]; z++)
			{
				Parkour( out_vertexes.size() / size_vertex, this->vertexes, this->triangles, tab,
					pos + (zero + z) * (size_x * size_y));
			}
			zero += (*ruban)[i + 1];
		}
		XY++;
	}

	out_vertexes.reserve(out_vertexes.size() + this->vertexes.size());
	out_vertexes.insert(out_vertexes.end(), this->vertexes.begin(), this->vertexes.end());

	out_triangles.reserve(out_triangles.size() + this->triangles.size());
	out_triangles.insert(out_triangles.end(), this->triangles.begin(), this->triangles.end());

}




		/*****	4 - getters	 	*****/

void Chunk::SetPos(int x, int y)
{
	this->pos_x = x;
	this->pos_y = y;
}

int Chunk::GetPos_x()
{
	return this->pos_x;
}

int Chunk::GetPos_y()
{
	return this->pos_y;
}



bool Chunk::SetData(u_char *tmp)
{
	if (!tmp || tmp == this->data)
		return false;

	delete [] this->data;
	this->data = tmp;
	return true;
}

u_char* Chunk::GetData()
{
	return this->data;
}

void	Chunk::SetRle(RLE &other)
{
	this->rle = other;
}

RLE&	Chunk::GetRle()
{
	return this->rle;
}


