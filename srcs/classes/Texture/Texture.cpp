#include <classes/Texture/Texture.hpp>  // Make sure to include the corresponding header file

Texture::Texture(u_int id, int width, int height, int nrChannels, u_char *data) : id(id), width(width), height(height), nrChannels(nrChannels), data(data) {}

u_char *Texture::GetData() {
	return data;
}

int Texture::GetWidth() {
	return width;
}

int Texture::GetHeight() {
	return height;
}

int Texture::GetNrChannel() {
	return nrChannels;
}

Texture::~Texture() {}