#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

class Texture
{
    public:
        Texture(u_int id, int width, int height, int nrChannels, u_char *data);
        ~Texture();

		u_char *GetData();
		int GetWidth();
		int GetHeight();
		int GetNrChannel();
    private:
		u_char *data;
        unsigned int id;
        int width;
        int height;
        int nrChannels;
};

#endif