#version 460 core

layout (location = 0) in int x;
layout (location = 1) in int y;
layout (location = 2) in int z;

out float vertexID;
uniform mat4 matrix;

void main()
{
	vertexID = gl_VertexID;
    gl_Position = matrix * vec4(x, z, -y, 1.0);
}