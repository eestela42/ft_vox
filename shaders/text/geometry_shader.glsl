#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
	uint geoChar;
	vec4 geoPos;
} geoStr[];

uniform int fontSize;
uniform int rowAmount;
uniform int columnAmount;

uniform float v[96];

out vec2 TexCoord;

void main()
{
	float textureX = float(geoStr[0].geoChar % columnAmount) / columnAmount + 1.0f / 128;
	float textureY = 1 - float(geoStr[0].geoChar / columnAmount) / rowAmount;
	float charWidth = v[geoStr[0].geoChar];
	float charHeight = 1.0f / rowAmount;

	TexCoord = vec2(textureX, textureY);
	gl_Position = geoStr[0].geoPos;
	EmitVertex();

	TexCoord = vec2(textureX + charWidth / 16, textureY);
	gl_Position = geoStr[0].geoPos + vec4(charWidth * fontSize / 1.5f / (1540.0f / 2),0,0,0);
	EmitVertex();

	TexCoord = vec2(textureX, textureY - charHeight);
	gl_Position = geoStr[0].geoPos + vec4(0, -(fontSize / (840.0f / 2)),0,0);
	EmitVertex();

	TexCoord = vec2(textureX + charWidth / 16, textureY - charHeight);
	gl_Position = geoStr[0].geoPos + vec4(charWidth * fontSize / 1.5f / (1540.0f / 2), -(fontSize / (840.0f / 2)),0,0);
	EmitVertex();
	
	EndPrimitive();
}