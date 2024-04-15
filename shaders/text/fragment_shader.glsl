#version 460 core

in vec2 TexCoord;
uniform sampler2D texture1;

out vec4 FragColor;

uniform int color;
uniform int rowAmount;
uniform int columnAmount;
uniform float xRatio;
uniform float yRatio;

void main() {
    FragColor = vec4(float((color >> 24) & 0xff) / 255, float((color >> 16) & 0xff) / 255, float((color >> 8) & 0xff) / 255, texture(texture1, TexCoord).w);
}