#include <classes/Texture/TextureLoader.hpp>

bool TextureLoader::isReady = false;

Texture TextureLoader::LoadTexture(std::string fileName) {
    if (!isReady) Init();

    int width, height, nrChannels;
    unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);

    if (data) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        return Texture(textureID, width, height, nrChannels);
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