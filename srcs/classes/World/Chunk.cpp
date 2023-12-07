#include <classes/World/Chunk.hpp>

Chunk::Chunk() : posX(0), posY(0) {
	
}

Chunk::Chunk(u_int x, u_int y) : posX(x), posY(y) {
	
}

void CreateCube(std::vector<float> &vData, std::vector<u_int> &iData, int x, int y, int z, int offset, int offsetX, int offsetY) {
	vData.push_back(0 + x + offsetX); 
    vData.push_back(0 + y + offsetY); 
    vData.push_back(0 + z); 

    //Other vertices
    vData.push_back(0 + x + offsetX); 
    vData.push_back(0 + y + offsetY); 
    vData.push_back(1 + z); 

    vData.push_back(1 + x + offsetX); 
    vData.push_back(0 + y + offsetY); 
    vData.push_back(1 + z); 

    vData.push_back(1 + x + offsetX); 
    vData.push_back(0 + y + offsetY); 
    vData.push_back(0 + z); 

    vData.push_back(1 + x + offsetX); 
    vData.push_back(1 + y + offsetY); 
    vData.push_back(0 + z); 

    vData.push_back(1 + x + offsetX); 
    vData.push_back(1 + y + offsetY); 
    vData.push_back(1 + z); 

    vData.push_back(0 + x + offsetX); 
    vData.push_back(1 + y + offsetY); 
    vData.push_back(1 + z); 

    vData.push_back(0 + x + offsetX); 
    vData.push_back(1 + y + offsetY); 
    vData.push_back(0 + z); 

    // Indices
    iData.push_back(0 + offset);
    iData.push_back(1 + offset);
    iData.push_back(2 + offset);

    iData.push_back(0 + offset);
    iData.push_back(2 + offset);
    iData.push_back(3 + offset);

    iData.push_back(2 + offset);
    iData.push_back(3 + offset);
    iData.push_back(4 + offset);

    iData.push_back(2 + offset);
    iData.push_back(4 + offset);
    iData.push_back(5 + offset);

    iData.push_back(4 + offset);
    iData.push_back(5 + offset);
    iData.push_back(6 + offset);

    iData.push_back(4 + offset);
    iData.push_back(6 + offset);
    iData.push_back(7 + offset);

    iData.push_back(6 + offset);
    iData.push_back(7 + offset);
    iData.push_back(0 + offset);

    iData.push_back(6 + offset);
    iData.push_back(0 + offset);
    iData.push_back(1 + offset);

    iData.push_back(1 + offset);
    iData.push_back(2 + offset);
    iData.push_back(5 + offset);

    iData.push_back(1 + offset);
    iData.push_back(5 + offset);
    iData.push_back(6 + offset);

    iData.push_back(0 + offset);
    iData.push_back(3 + offset);
    iData.push_back(4 + offset);

    iData.push_back(0 + offset);
    iData.push_back(4 + offset);
    iData.push_back(7 + offset);
}

void Chunk::CompileData() {
	for (int z = 0; z < sizeZ; z++) {
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				if (data[z * sizeY * sizeX + y * sizeX + x]) {

					CreateCube(vertexData, shapeAssemblyData, x, y, z, (z * sizeY * sizeX + y * sizeX + x) * 8, posX * sizeX, posY * sizeY);
				}
			}
		}
	}
}

void Chunk::Generate() {
	data = (u_char*)calloc(sizeX * sizeY * sizeZ, sizeof(*data));
	for (int z = 0; z < sizeZ; z++) {
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				if(z < posY + posX + 1) {
					data[z * sizeY * sizeX + y * sizeX + x] = 1;
				}
			}
		}
	}
}

std::vector<float>&	Chunk::GetVertexData() {
	return vertexData;
}

std::vector<u_int>&		Chunk::GetShapeAssemblyData() {
	return shapeAssemblyData;
}

Chunk::~Chunk() {}