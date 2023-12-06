#include <classes/World/Chunk.hpp>

Chunk::Chunk() {
	Generate();
	CompileData();
}

void CreateCube(std::vector<float> &vData, std::vector<u_int> &iData, int x, int y, int z, int offset) {
	std::cout << "test1 : " << vData.capacity() << std::endl;
	vData.push_back(0 + x); //x
    vData.push_back(0 + y); //y
    vData.push_back(0 + z); //z

    // Other vertices
    vData.push_back(0 + x); //x
    vData.push_back(0 + y); //y
    vData.push_back(1 + z); //z

    vData.push_back(1 + x); //x
    vData.push_back(0 + y); //y
    vData.push_back(1 + z); //z

    vData.push_back(1 + x); //x
    vData.push_back(0 + y); //y
    vData.push_back(0 + z); //z

    vData.push_back(1 + x); //x
    vData.push_back(1 + y); //y
    vData.push_back(0 + z); //z

    vData.push_back(1 + x); //x
    vData.push_back(1 + y); //y
    vData.push_back(1 + z); //z

    vData.push_back(0 + x); //x
    vData.push_back(1 + y); //y
    vData.push_back(1 + z); //z

    vData.push_back(0 + x); //x
    vData.push_back(1 + y); //y
    vData.push_back(0 + z); //z

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
	std::cout << "Max size : " << vertexData.max_size() << std::endl;
	//vertexData.reserve(sizeX * sizeY * sizeZ * 8 * sizeof(float) * 3); //Chunk size * vertices * floatSize * floatAmount
	//shapeAssemblyData.reserve(sizeX * sizeY * sizeZ * 12 * sizeof(u_int) * 3); //Chunk size * primitives * intSize * u_intAmount
	for (int z = 0; z < sizeZ; z++) {
		for (int y = 0; y < sizeY; y++) {
			for (int x = 0; x < sizeX; x++) {
				if (data[z * sizeY * sizeX + y * sizeX + x]) {
					std::cout << "z : " << z << " | y : " << y << " | x : " << x << std::endl;
					CreateCube(vertexData, shapeAssemblyData, x, y, z, (z * sizeY * sizeX + y * sizeX + x) * 8);
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
				if(z < 50) {
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