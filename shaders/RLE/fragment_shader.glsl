#version 460 core
out vec4 FragColor;

in float vertexID;
in vec2 TexCoord;
in float grass;


uniform sampler2D ourTexture;

void main()
{
	// FragColor = vec4(float(int(vertexID) % 256) / 256, float(int(vertexID) % (256 * 256) / 256) / 256 , float(int(vertexID) / (256 * 256)) / 256, 1);
	if (grass == 1)
		FragColor = texture(ourTexture, TexCoord) * vec4(0.4, 1, 0.4, 1);
	else
		FragColor = texture(ourTexture, TexCoord);
}
