#ifndef RLE_HPP
# define RLE_HPP

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
# include <math.h>
# include <chrono>

	
class RLE
{

private :

	u_int 							size_x = 16;
	u_int 							size_y = 16;
	u_int 							size_z = 256;
	std::vector<std::vector<u_char>>	rubans;


public :

		/*****	1 - constructors 		*****/

	~RLE();
	RLE();
	RLE(const RLE &other);
	RLE(u_int x, u_int y, u_int z);
	RLE(u_int x, u_int y, u_int z, u_char type);
	RLE(float seed);

		/*****	2 - fillers	 		*****/

	void				 			CreateFromData(u_char *data);
	u_char*							CreateDataFromRle();

		/*****	3 - data getters 		*****/

	std::vector<std::vector<u_char>>&	GetRubans();
	
		/*****	4 - operators 		*****/

	void 							Print();

	RLE& 							operator=(const RLE &other);
};



#endif