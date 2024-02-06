#version 460 core
out vec4 FragColor;

in float out_light;
in vec2 tex_out;
in float out_grass;


uniform sampler2D ourTexture;

void main()
{
	// FragColor = vec4(float(int(vertexID) % 256) / 256, float(int(vertexID) % (256 * 256) / 256) / 256 , float(int(vertexID) / (256 * 256)) / 256, 1);
	if (out_grass == 1)
		FragColor = texture(ourTexture, tex_out) * vec4(0.4, 1, 0.4, 1);
	else
		FragColor = texture(ourTexture, tex_out);
	// FragColor = vec4(0.3, 0.1, 0.4, 1);
}
