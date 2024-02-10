#version 460 core

layout (location = 0) in vec3 pos;


out vec2 TexCoord;
uniform mat4 matrix;
uniform float playerPosX;
uniform float playerPosY;
uniform float playerPosZ;

void main()
{
	// vertexID = gl_VertexID;
    // gl_Position = matrix * (vec4(pos.x, pos.y, pos.z, 1.0));
	
	TexCoord = vec2(pos.x, pos.y);
    gl_Position = matrix * (vec4(pos.x + playerPosX, pos.y + playerPosY, pos.z + playerPosZ, 1.0));
}