#ifndef SKYBOX_HPP
# define SKYBOX_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
# include <stdio.h>

class skyBox
{
	public:
		skyBox();
		~skyBox();

		void replace(int x, int y);
		void setRenderDistance(unsigned int renderDistance);
		std::vector<int> data;
		std::vector<unsigned int> indices = {0, 1, 2};
	private:
		unsigned int renderDistance;
};



#endif