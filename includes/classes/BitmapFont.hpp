#pragma once

#include <classes/Texture/TextureLoader.hpp>
#include <vector>
#include <classes/Shader.hpp>

class BitmapFont {
	private:
		Texture texture;
		std::vector<u_char> widthList;
		Shader *shader;
		int width;
		int height;
		int nrChannels;
		int rowAmount;
		int columnAmount;
	public:
		BitmapFont(Shader *shader, std::string fileName, u_int rowNr, u_int columnNr, u_int startChar);

		std::vector<u_char> &GetWidthList();
		Shader *GetShader();
		int GetTextureWidth();
		int GetTextureHeight();
		int GetRowAmount();
		int GetColumnAmount();
};