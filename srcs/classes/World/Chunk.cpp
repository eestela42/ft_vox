#include <classes/World/Chunk.hpp>

std::vector<std::vector<Chunk*>> Chunk::loadedChunks;

Chunk::Chunk() : posX(0), posY(0){}

Chunk::Chunk(int posX, int posY) : posX(posX), posY(posY) {
	loadChunk();
}

void Chunk::PublicGenerate() {
	std::cout << "Generating chunk " << posX << " " << posY << std::endl;
	isGenerated = true;
	Generate();
	isCompiled = false;
}

void Chunk::PublicGenerate(std::vector<glm::ivec3> positionList, std::vector<glm::ivec3> sizeList) {
	isGenerated = true;
	Generate(positionList, sizeList);
	isCompiled = false;
}

void Chunk::PublicGenerate(u_int seed) {
	isGenerated = true;
	Generate(seed);
	isCompiled = false;
}

void	Chunk::loadChunk()
{
	if (loadedChunks.size()) {
		loadedChunks[posX % loadedChunks.size()][posY % loadedChunks.size()] = this;
	}
}

void	Chunk::setRenderDistance(int renderDistance)
{
	if (renderDistance == loadedChunks.size()) {
		return ;
	}
	std::vector<std::vector<Chunk*>> oldChunks = loadedChunks;

	loadedChunks.assign(renderDistance * 2 + 1, std::vector<Chunk*>());
	for (int x = 0; x < renderDistance * 2 + 1; x++)
	{
		loadedChunks[x].resize(renderDistance * 2 + 1, nullptr);
		for (int y = 0; y < renderDistance * 2 + 1; y++)
			loadedChunks[x][y] = NULL;
	}

	if (renderDistance > oldChunks.size()) {
		for (std::vector<std::vector<Chunk*>>::iterator iterator = oldChunks.begin(); iterator != oldChunks.end(); iterator++) {
			for (std::vector<Chunk*>::iterator iterator2 = iterator->begin(); iterator2 != iterator->end(); iterator2++) {
				(*iterator2)->loadChunk();
			}
		}
	}
}

Chunk *Chunk::GetNeighbor(int x, int y) {
	return (loadedChunks[x % loadedChunks.size()][y % loadedChunks.size()]);
}

void Chunk::SetReady() {
	Chunk *neighborChunk = GetNeighbor(posX, posY + 1);
	if (neighborChunk != neighborChunks[0])
		isCompiled = false;
	neighborChunks[0] = neighborChunk;

	neighborChunk = GetNeighbor(posX + 1, posY);
	if (neighborChunk != neighborChunks[1])
		isCompiled = false;
	neighborChunks[1] = neighborChunk;

	neighborChunk = GetNeighbor(posX, posY - 1);
	if (neighborChunk != neighborChunks[2])
		isCompiled = false;
	neighborChunks[2] = neighborChunk;

	neighborChunk = GetNeighbor(posX - 1, posY);
	if (neighborChunk != neighborChunks[3])
		isCompiled = false;
	neighborChunks[3] = neighborChunk;
	
	if (!isGenerated) {
		PublicGenerate();
	}
	if (!isCompiled) {
		isCompiled = true;
		for (int i = 0; i < 4; i++) {
			if (neighborChunks[i]) {
				neighborChunks[i]->SetReady();
			}
		}
		CompileData();
		didUpdate = true;
	}
}

bool Chunk::IsGenerated() {
	return isGenerated;
}

bool Chunk::DidUpdate() {
	return didUpdate;
}

std::vector<float>&	Chunk::GetVertexData() {
	SetReady();
	didUpdate = false;
	return vertexData;
}

std::vector<u_int>&		Chunk::GetShapeAssemblyData() {
	SetReady();
	didUpdate = false;
	return shapeAssemblyData;
}

Chunk::~Chunk() {}