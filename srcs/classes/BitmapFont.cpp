#include <classes/BitmapFont.hpp>
#include <classes/Shader.hpp>

BitmapFont::BitmapFont(Shader *shader, std::string fileName, u_int rowAmount, u_int columnAmount, u_int startChar) : texture(TextureLoader::LoadTexture(fileName, true, true)), shader(shader), rowAmount(rowAmount), columnAmount(columnAmount) {
	u_char *data = texture.GetData();
	width = texture.GetWidth();
	height = texture.GetHeight();
	nrChannels = 4;
	u_char a;
	int i;
	widthList.resize(rowAmount * columnAmount,0);

	for (int y = 0; y < height; y++) {
	for (int x = 0; x < width; x++) {
		a = data[y * width * nrChannels + x * nrChannels + 3];
		if (a) {
			i = (height - 1 - y) / (height / rowAmount) * columnAmount + (x / (width / columnAmount));
			if (widthList[i] < x % (width / columnAmount)) {
				widthList[i] = x % (width / columnAmount);
			}
		}
	}
	}
	widthList[0] = 2;
}

std::vector<u_char> &BitmapFont::GetWidthList() {
	return widthList;
}

Shader *BitmapFont::GetShader() {
	return shader;
}

int BitmapFont::GetTextureWidth() {
	return width;
}
int BitmapFont::GetTextureHeight() {
	return height;
}

int BitmapFont::GetRowAmount() {
	return rowAmount;
}
int BitmapFont::GetColumnAmount() {
	return columnAmount;
}