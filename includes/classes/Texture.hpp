#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

class Texture
{
    public:
        Texture(u_int id, int width, int height, int nrChannels);
        ~Texture();

    private:
        unsigned int id;
        int width;
        int height;
        int nrChannels;
};

#endif