#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in uint mychar;

out VS_OUT {
	uint geoChar;
	vec4 geoPos;
} geoStrOut;

void main()
{
    geoStrOut.geoPos = vec4(aPos.x, aPos.y, 0.0, 1.0);
	geoStrOut.geoChar = mychar - 32;
}