#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;


in GEO_OUT {
	int chunk_x;
	int chunk_y;
	int pos;
	int face;
	int type;
} geo_in[];


uniform float size_texture = 128;
uniform float width_Texture = 2048;

uniform int chunk_size_x;
uniform int chunk_size_y;

uniform mat4 matrix;
uniform vec4 cameraPos;

out vec2 tex_out;
flat out int out_grass;
out float out_light;


void create_vertex(vec3 pos, vec2 texCoords)
{
	gl_Position = matrix * vec4(pos, 1);
	tex_out = texCoords;
	EmitVertex();
}

void create_face(vec3 pos, vec2 zero_texture, int face)
{
	vec2 texCoords[4];
	texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
	texCoords[1] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + 0.0);
	texCoords[2] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + size_texture / width_Texture);
	texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture / width_Texture);

	
	switch (face)
	{
	case 0 :
		
		create_vertex(pos + vec3(0.0, 0.0, 1.0), texCoords[0]);
		create_vertex(pos + vec3(1.0, 0.0, 1.0), texCoords[1]);
		create_vertex(pos + vec3(0.0, 1.0, 1.0), texCoords[3]);
		create_vertex(pos + vec3(1.0, 1.0, 1.0), texCoords[2]);
		
		break;
	case  1 :
		create_vertex(pos + vec3(1.0, 0.0, 0.0), texCoords[0]);
		create_vertex(pos + vec3(1.0, 0.0, 1.0), texCoords[1]);
		create_vertex(pos + vec3(1.0, 1.0, 0.0), texCoords[3]);
		create_vertex(pos + vec3(1.0, 1.0, 1.0), texCoords[2]);

		break;
	case 2 :
		create_vertex(pos + vec3(0.0, 0.0, 0.0), texCoords[0]);
		create_vertex(pos + vec3(1.0, 0.0, 0.0), texCoords[1]);
		create_vertex(pos + vec3(0.0, 1.0, 0.0), texCoords[3]);
		create_vertex(pos + vec3(1.0, 1.0, 0.0), texCoords[2]);
		
		break;
	
	case 3 :
		create_vertex(pos + vec3(0.0, 0.0, 0.0), texCoords[0]);
		create_vertex(pos + vec3(0.0, 0.0, 1.0), texCoords[1]);
		create_vertex(pos + vec3(0.0, 1.0, 0.0), texCoords[3]);
		create_vertex(pos + vec3(0.0, 1.0, 1.0), texCoords[2]);

		break;
	
	case  4 :
		create_vertex(pos + vec3(0.0, 0.0, 0.0), texCoords[0]);
		create_vertex(pos + vec3(1.0, 0.0, 0.0), texCoords[1]);
		create_vertex(pos + vec3(0.0, 0.0, 1.0), texCoords[3]);
		create_vertex(pos + vec3(1.0, 0.0, 1.0), texCoords[2]);

		break;
	case  5 :
		create_vertex(pos + vec3(0.0, 1.0, 0.0), texCoords[0]);
		create_vertex(pos + vec3(1.0, 1.0, 0.0), texCoords[1]);
		create_vertex(pos + vec3(0.0, 1.0, 1.0), texCoords[3]);
		create_vertex(pos + vec3(1.0, 1.0, 1.0), texCoords[2]);

		break;
	}
	EndPrimitive();
}

void main() {
	int pos_block = geo_in[0].pos;
	int x = pos_block % chunk_size_x;
	int y = (pos_block % (chunk_size_x * chunk_size_y)) / chunk_size_y;
	int z = pos_block / (chunk_size_x * chunk_size_y);

	x += geo_in[0].chunk_x * chunk_size_x;
	y += geo_in[0].chunk_y * chunk_size_x;
	vec3 pos = vec3(x, z, y) - cameraPos.xyz;

	vec2 zero_texture = vec2(0.0, 0.0);
	int vtype = geo_in[0].type;
	out_grass = 0;
	if (vtype == 19 && geo_in[0].face == 5)
	{
		vtype = 16;
		out_grass = 1;
	}
	else if (vtype == 68 || vtype == 69)
		out_grass = 1;

	zero_texture = vec2((vtype % 16 * 128) / width_Texture,
						1 - (vtype / 16 * 128) / width_Texture);

	
	if (geo_in[0].face == 2 || geo_in[0].face == 3 || geo_in[0].face == 4)
			out_light = 0.8;
		else
			out_light = 1.0;
	create_face(pos, zero_texture, geo_in[0].face);

}


	

	



	
	