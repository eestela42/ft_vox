#version 460 core
layout (location = 0) in int data1; // 3 byes
layout (location = 1) in int data2; // 3 byes
layout (location = 2) in int data3; // 10 bits = 2 bytes + 2 bits
 // 2 bytes


out vec2 TexCoord;
out float vertexID;

uniform float size_texture = 128;
uniform float width_Texture = 2048;

uniform int chunk_size_x = 16;
uniform int chunk_size_y = 16;

uniform mat4 matrix;


void main()
{


	int chunk_x = 0;
	int chunk_y = 0;
	int pos = 0;
	int face = 0;
	int type = 0;
	int point = 0;
	point = gl_VertexID % 4;
//XXXY YYPP P0FT
	chunk_x = data1 >> 8;
	chunk_y = (data1 << 16) & 0x00FF0000;
	chunk_y = (data2 >> 16) & 0x0000FFFF; 
	pos = (data2 & 0x0000FFFF) << 2;
	pos += (data3 & 0xC0000000) >> 30;
	face = (data3 >> 22) & 0x000000FF;
	type = (data3 >> 14) & 0x000000FF;

	vec2 zero_texture = vec2(0.0, 0.0);


	float tmp = ((type - 1) * size_texture) / width_Texture;
	zero_texture = vec2(tmp - floor(tmp),
						tmp / width_Texture);

	vec2 texCoords[4];
	
	vec3 points[4];

	switch (face)
	{
	case 0 :
		points[0] = vec3(0.0, 0.0, 1.0);
		points[1] = vec3(1.0, 0.0, 1.0);
		points[2] = vec3(1.0, 1.0, 1.0);
		points[3] =  vec3(0.0, 1.0, 1.0);

		
		texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
		texCoords[1] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + 0.0);
		texCoords[2] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + size_texture / width_Texture);
		texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture / width_Texture);
		break;
	case  1 : 
		points[0] = vec3(1.0, 0.0, 0.0);
		points[1] = vec3(1.0, 0.0, 1.0);
		points[2] = vec3(1.0, 1.0, 1.0);
		points[3] =  vec3(1.0, 1.0, 0.0);

		texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
		texCoords[1] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + 0.0);
		texCoords[2] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + size_texture / width_Texture);
		texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture / width_Texture);
		break;
	case 2:
		points[0] = vec3(0.0, 0.0, 0.0);
		points[1] = vec3(1.0, 0.0, 0.0);
		points[2] = vec3(1.0, 1.0, 0.0);
		points[3] =  vec3(0.0, 1.0, 0.0);

		texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
		texCoords[1] = vec2(zero_texture.x + size_texture/ width_Texture, zero_texture.y + 0.0);
		texCoords[2] = vec2(zero_texture.x + size_texture/ width_Texture, zero_texture.y + size_texture/ width_Texture);
		texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture/ width_Texture);
		break;
	
	case 3 :
		points[0] = vec3(0.0, 0.0, 0.0);
		points[1] = vec3(0.0, 0.0, 1.0);
		points[2] = vec3(0.0, 1.0, 1.0);
		points[3] = vec3(0.0, 1.0, 0.0);

		texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
		texCoords[1] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + 0.0);
		texCoords[2] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + size_texture / width_Texture);
		texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture / width_Texture);
		break;
	
	case  4 :
		points[0] = vec3(0.0, 0.0, 0.0);
		points[1] = vec3(1.0, 0.0, 0.0);
		points[2] = vec3(1.0, 0.0, 1.0);
		points[3] =  vec3(0.0, 0.0, 1.0);

		texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
		texCoords[1] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + 0.0);
		texCoords[2] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + size_texture / width_Texture);
		texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture / width_Texture);
		break;
	case  5 : 
		points[0] = vec3(0.0, 1.0, 0.0);
		points[1] = vec3(1.0, 1.0, 0.0);
		points[2] = vec3(1.0, 1.0, 1.0);
		points[3] =  vec3(0.0, 1.0, 1.0);

		texCoords[0] = vec2(zero_texture.x + 0.0, zero_texture.y + 0.0);
		texCoords[1] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + 0.0);
		texCoords[2] = vec2(zero_texture.x + size_texture / width_Texture, zero_texture.y + size_texture / width_Texture);
		texCoords[3] = vec2(zero_texture.x + 0.0, zero_texture.y + size_texture / width_Texture);
		break;
	}

    vec3 newPos;

	int pos_block = pos;
	int x = pos_block % chunk_size_x;
	int y = (pos_block % (chunk_size_x * chunk_size_y)) / chunk_size_y;
	int z = pos_block / (chunk_size_x * chunk_size_y);

	x += chunk_x * chunk_size_x;
	y += chunk_y * chunk_size_x;

	newPos.x = x + points[point].x;
	newPos.y = z + points[point].y;
	newPos.z = y + points[point].z;
	
	newPos.y = newPos.y;

	gl_Position = matrix * vec4(newPos, 1.0);
	
	vertexID = gl_VertexID;
	TexCoord = texCoords[point];
	// ourColor = vec3(0.0, 1.0, 1.0);
}
