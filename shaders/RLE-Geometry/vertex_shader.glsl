#version 460 core
layout (location = 0) in int chunk_x; 
layout (location = 1) in int chunk_y; 
// layout (location = 2) in int data;
layout (location = 2) in int pos; 
layout (location = 3) in int face; 
layout (location = 4) in int type;
layout (location = 5) in int longX;
layout (location = 6) in int longY;




uniform float size_texture = 128;
uniform float width_Texture = 2048;

uniform int chunk_size_x;
uniform int chunk_size_y;
uniform vec4 cameraDir;
uniform mat4 matrix;



out GEO_OUT {
	int chunk_x;
	int chunk_y;
	int pos;
	int face;
	int type;
	int longX;
	int longY;
} geo_out;

out int tmp_vertID;

float scalar(vec3 a, vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

void main()
{
	tmp_vertID = gl_VertexID;

	geo_out.chunk_x = chunk_x;
	geo_out.chunk_y = chunk_y;

	// geo_out.pos = data >> 16  & 0xFFFF;
	// geo_out.face = data >> 12 & 0xF;
	// geo_out.type = data >> 8 & 0xF;
	// geo_out.longX = data >> 4 & 0xF;
	// geo_out.longY = data & 0xF;

	geo_out.pos = pos;
	geo_out.face = face;
	geo_out.type = type;
	geo_out.longX = longX;
	geo_out.longY = longY;

	// vec3 normal;
	// if (face == 0)
	// 	normal = vec3(1, 0, 0);
	// else if (face == 1)
	// 	normal = vec3(-1, 0, 0);
	// else if (face == 2)
	// 	normal = vec3(0, 0, 1);
	// else if (face == 3)
	// 	normal = vec3(0, 0, -1);
	// else if (face == 4)
	// 	normal = vec3(0, 1, 0);
	// else if (face == 5)
	// 	normal = vec3(0, -1, 0);


	// vec3 newDir = vec3(vec4(cameraDir.xyz, 0.0));
	// float cosAngle = dot(normal, newDir);
    // if (cosAngle < 0.0) {
	// 	geo_out.type = 1000 ;
	// }
	
}
