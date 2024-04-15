#include <classes/Texture/TextureLoader.hpp>

bool TextureLoader::isReady = false;

Texture TextureLoader::LoadTexture(std::string fileName, bool SaveData, bool blackTransparent) {
    if (!isReady) Init();

    int width, height, nrChannels;
	u_int colorType;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
	colorType = (nrChannels == 4) ? GL_RGBA : GL_RGB;
	if (colorType == GL_RGBA && blackTransparent) {
		for (int i = 0; i < width * height; i++) {
			if (((u_int*)data)[i] == 0xff000000) {
				((u_int*)data)[i] = 0;
			}
		}
 	}
    if (data) {
        unsigned int textureID;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, colorType, width, height, 0, colorType, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

		if (SaveData) {
			return Texture(textureID, width, height, nrChannels, data);
		}
		else {
			stbi_image_free(data);
			return Texture(textureID, width, height, nrChannels, 0);
		}
    } else {
        std::cout << "Failed to load texture : " << fileName << std::endl;
        assert(!"TextureLoader::LoadTexture stbi failed to load the texture");
    }
}

void TextureLoader::Init() {
    if (!glfwGetCurrentContext()) {
        std::cout << "TextureLoader cannot be used before a glfw context has been initialized" << std::endl;
        assert(!"TextureLoader::Init() glfwGetCurrentContext failed to load the current context");
    }
    stbi_set_flip_vertically_on_load(true);
    isReady = true;
}