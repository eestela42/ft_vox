#version 460 core
out vec4 FragColor;

in float vertexID;
in vec2 TexCoord;


uniform sampler2D ourTexture;

void main()
{
	// FragColor = vec4(float(int(vertexID) % 256) / 256, float(int(vertexID) % (256 * 256) / 256) / 256 , float(int(vertexID) / (256 * 256)) / 256, 1);
	FragColor = texture(ourTexture, TexCoord);
}
