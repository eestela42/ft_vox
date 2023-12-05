#ifndef MAIN_H
# define MAIN_H

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>
# include <glm/gtc/quaternion.hpp>
# include <glm/gtx/quaternion.hpp>

		/*****	 World	*****/

# include <classes/World/RLE.hpp>
# include <classes/World/PerlinNoise.hpp>
# include <classes/World/Chunk.hpp>


		/*****	 Game		*****/

# include <classes/Game/Game.hpp>
#include <classes/Game/InputHandler.hpp>


		/*****	 Texture	*****/

# include <classes/Texture/TextureLoader.hpp>

		/*****	 VAO		*****/		

#include <classes/VAO/VertexArrayObject.hpp>
#include <classes/VAO/VertexArrayObjectHandler.hpp>

#include <classes/Shader.hpp>
#include <classes/Window.hpp>

#include <iostream>

#endif