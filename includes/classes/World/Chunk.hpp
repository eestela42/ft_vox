#ifndef CHUNK_HPP
# define CHUNK_HPP


#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>


# ifndef RLE_HPP
#  include "RLE.hpp"
# endif



class Chunk
{
	
private :

	u_char*							data;
	RLE								rle;

	int 							size_x = 16;
	int 							size_y = 16;
	int 							size_z = 256;
	int 							pos_x;
	int 							pos_y;
	int 							size_vertex = 3;

	std::vector<unsigned int> 		triangles;
	std::vector<int> 				vertexes;

public :

		/*****	1 - constructors 		*****/

	~Chunk();
	Chunk();
	Chunk(int x, int y);
	Chunk(int x, int y, RLE &other);
	Chunk(int x, int y, float seed);

		/*****	2 - fillers	 			*****/


		/*****	3 - toVbo		 		*****/				
	
	void 							CreatePointVertex(std::vector<int> &vertexes, int pos, u_char orientation, u_char type);
	void 							CreateTrianglesFace(int start, std::vector<unsigned int> &triangles);
	
	void 							Parkour(int start_vert, std::vector<int> &vertexes,std::vector<unsigned int> &triangles, bool *tab, int pos);
	
	void 							RleToVbo(std::vector<int> &out_vertexes, std::vector<unsigned int> &out_triangles);

		/*****	4 - getters	 			*****/


	void 							SetPos(int x, int y);
	int 							GetPos_x();
	int 							GetPos_y();

	bool 							SetData(u_char *tmp);
	u_char*							GetData();

	void							SetRle(RLE &other);
	RLE&							GetRle();


};




#endif