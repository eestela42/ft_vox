#version 460 core

layout (location = 0) in vec3 aPos;

out float vertexID;

uniform mat4 matrix;

void main()
{
	vertexID = gl_VertexID;
    gl_Position = matrix * vec4(aPos.x, aPos.z, aPos.y, 1.0);
}