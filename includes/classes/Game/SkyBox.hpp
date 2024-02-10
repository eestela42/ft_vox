#ifndef SKYBOX_HPP
# define SKYBOX_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
# include <stdio.h>

#include <classes/VAO/VertexArrayObjectHandler.hpp>
#include <classes/Texture/Texture.hpp>


class SkyBox
{
	public:
		SkyBox(Shader *skyBoxShader, Texture skyBoxTexture);
		~SkyBox();

		void replace(int x, int y);
		void setRenderDistance(unsigned int renderDistance);
		std::vector<unsigned int> indices = {0, 1, 2};
		VertexArrayObject *VAO;
		Shader *shader;
		std::vector<float> vertices;
		std::vector<u_int> shape;
		Texture texture;
		

	private:
};





#endif