#version 460 core
out vec4 FragColor;

in float out_light;
in vec2 tex_out;
flat in int out_grass;


uniform sampler2D ourTexture;
uniform vec4 colorFilter;

void main()
{
	// FragColor = vec4(float(int(vertexID) % 256) / 256, float(int(vertexID) % (256 * 256) / 256) / 256 , float(int(vertexID) / (256 * 256)) / 256, 1);
	if (out_grass == 1)
	{
		FragColor = texture(ourTexture, tex_out) * vec4(0.4, 0.9, 0.6, 0);
		return;
	}
	// if (out_grass == 2)
	// {
	// 	FragColor += vec4(0, 0, 0.5, 0.3);
	// 	return;
	// }
	FragColor = texture(ourTexture, tex_out) * out_light * colorFilter;
}
