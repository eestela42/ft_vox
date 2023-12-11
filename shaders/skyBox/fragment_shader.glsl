#version 460 core

in float vertexID;

out vec4 FragColor;

void main() {
    FragColor = vec4(float(int(vertexID) % 256) / 256, float(int(vertexID) % (256 * 256) / 256) / 256 , float(int(vertexID) / (256 * 256)) / 256, 1);
}