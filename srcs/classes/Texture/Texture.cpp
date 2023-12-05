#include <classes/Texture/Texture.hpp>  // Make sure to include the corresponding header file

Texture::Texture(u_int id, int width, int height, int nrChannels) : id(id), width(width), height(height), nrChannels(nrChannels) {}

Texture::~Texture() {}