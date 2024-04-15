#pragma once

#include <classes/BitmapFont.hpp>
#include <classes/Window.hpp>
#include <classes/Shader.hpp>
#include <classes/vertexData.hpp>

class Text {
	private:
		std::vector<float> *floatVector;
		std::vector<u_char> *textVector = new std::vector<u_char>();
		std::string text;
		Window *window;
		BitmapFont *font;
		Shader *shader;
		int color;
		int fontSize;
		float posX;
		float posY;
		bool hasUpdated = false;

		t_vertexData vertexData;

		void CompileData();
	public:
		Text(Window *window, BitmapFont *font, int color, int fontSize, std::string text, float posX, float posY);

		void ChangeText(std::string newText);
		bool HasUpdated();
		void SetUniforms();
		Shader *GetShader();
		t_vertexData &GetVertexData();
};