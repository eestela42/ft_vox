#include <classes/World/ChunkDefault.hpp>

ChunkDefault::ChunkDefault() : Chunk(0, 0) {}

ChunkDefault::ChunkDefault(u_int x, u_int y) : Chunk(x, y) {}

void ChunkDefault::CreateCube(std::vector<float> &vData, std::vector<u_int> &iData, int x, int y, int z, int offset, int offsetX, int offsetY) {
	
	offset = vData.size() / 3;
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

void ChunkDefault::CompileData() {
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

bool ChunkDefault::isFilled(int x, int y, int z) {
	if (!IsGenerated()) {
		return false;
	}
	return data[z * sizeY * sizeX + y * sizeX + x];
}

void ChunkDefault::Generate() {
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

void ChunkDefault::Generate(std::vector<glm::ivec3> positionList, std::vector<glm::ivec3> sizeList) {}

void ChunkDefault::Generate(u_int seed) {}