#include <classes/Text.hpp>

Text::Text(Window *window, BitmapFont *font, int color, int fontSize, std::string text, float posX, float posY) : window(window), font(font), color(color), fontSize(fontSize), text(text), posX(posX), posY(posY) {
	shader = font->GetShader();
	
	shader->Use();
	shader->SetInt("rowAmount", 6);
	shader->SetInt("columnAmount", 16);

	floatVector = new std::vector<float>();
	for (auto const x: font->GetWidthList()) {
		floatVector->push_back((float)x / 8);
	}
	shader->SetFloatArray("v", floatVector->data(), 96);

	CompileData();
}

void Text::CompileData() {
	float charPosX = 0;
	textVector->clear();
	textVector->resize(text.length() * 12);
	for (int i = 0; i < text.length(); i++) {
		((float*)textVector->data())[0 + i * 3] = posX + charPosX;
		((float*)textVector->data())[1 + i * 3] = posY;
		textVector->data()[8 + i * 12] = text[i];
		charPosX += ((*floatVector)[text[i] - 32] + 0.10f) * fontSize / 1.5f / (1540.0f / 2);
	}
	vertexData.data = textVector->data();
	vertexData.size = textVector->size();
}

void Text::ChangeText(std::string newText) {
	text = newText;
	CompileData();
	hasUpdated = true;
}

bool Text::HasUpdated() {
	return hasUpdated;
}

void Text::SetUniforms()
{
	shader->Use();
	shader->SetInt("color", color);
	shader->SetInt("fontSize", fontSize);
}

Shader *Text::GetShader()
{
	return shader;
}
t_vertexData &Text::GetVertexData()
{
	hasUpdated = false;
	return vertexData;
}