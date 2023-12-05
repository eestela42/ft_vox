#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <classes/Texture.hpp>
#include <stb/stb_image.h>

class TextureLoader
{
    public:
        static Texture LoadTexture(std::string fileName);
    private:
        static bool isReady;
        static void Init();
};

#endif