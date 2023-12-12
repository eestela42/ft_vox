#include <classes/World/Chunk.hpp>

std::vector<std::vector<Chunk*>> Chunk::loadedChunks;
u_int Chunk::idCount = 0;

Chunk::Chunk() : posX(0), posY(0){}

Chunk::Chunk(int posX, int posY) : posX(posX), posY(posY) {
	idCount++;
	id = idCount;
	loadChunk();
}

void Chunk::PublicGenerate() {
	Profiler::StartTracking("Chunk::PublicGenerate()");
	isGenerated = true;
	Generate();
	isCompiled = false;
	Profiler::StopTracking("Chunk::PublicGenerate()");
}

void Chunk::PublicGenerate(std::vector<glm::ivec3> positionList, std::vector<glm::ivec3> sizeList) {
	Profiler::StartTracking("Chunk::PublicGenerate(positionList, sizeList)");
	isGenerated = true;
	Generate(positionList, sizeList);
	isCompiled = false;
	Profiler::StopTracking("Chunk::PublicGenerate(positionList, sizeList)");
}

void Chunk::PublicGenerate(u_int seed) {
	Profiler::StartTracking("Chunk::PublicGenerate(seed)");
	isGenerated = true;
	Generate(seed);
	isCompiled = false;
	Profiler::StopTracking("Chunk::PublicGenerate(seed)");
}

void	Chunk::loadChunk() {
	int size = loadedChunks.size();
	if (size) {
		loadedChunks[(posX % size + size) % size][(posY % size + size) % size] = this;
	}
}

void	Chunk::UnloadChunk() {
	int size = loadedChunks.size();
	if (size) {
		loadedChunks[(posX % size + size) % size][(posY % size + size) % size] = NULL;
	}
}

void	Chunk::setRenderDistance(int renderDistance) {
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

const std::vector<std::vector<Chunk*>> &Chunk::GetLoadedChunks() {
	return loadedChunks;
}

bool Chunk::IsRealNeighbor(int chunkX, int chunkY) {
	if ((std::abs(chunkX - posX) == 1 && chunkY == posY) || (std::abs(chunkY - posY) == 1 && chunkX == posX)) {
		return true;
	}
	return false;
}

Chunk *Chunk::GetNeighbor(int x, int y) {
	int size = loadedChunks.size();
	return loadedChunks[(x % size + size) % size][(y % size + size) % size];
}

void Chunk::SetReady() {
	bool newNegh = false;
	neighborChunks[0] = GetNeighbor(posX, posY + 1);
	u_int neighborID = (neighborChunks[0]) ? neighborChunks[0]->id : 0;
	if (neighborID != neighborChunksID[0]) {
		newNegh = true;
		isCompiled = false;
	}
	neighborChunksID[0] = neighborID;

	neighborChunks[1] = GetNeighbor(posX + 1, posY);
	neighborID = (neighborChunks[1]) ? neighborChunks[1]->id : 0;
	if (neighborID != neighborChunksID[1]){
newNegh = true;
		isCompiled = false;
	}
	neighborChunksID[1] = neighborID;

	neighborChunks[2] = GetNeighbor(posX, posY - 1);
	neighborID = (neighborChunks[2]) ? neighborChunks[2]->id : 0;
	if (neighborID != neighborChunksID[2]){
newNegh = true;
		isCompiled = false;
	}
	neighborChunksID[2] = neighborID;

	neighborChunks[3] = GetNeighbor(posX - 1, posY);
	neighborID = (neighborChunks[3]) ? neighborChunks[3]->id : 0;
	if (neighborID != neighborChunksID[3]){
newNegh = true;
		isCompiled = false;
	}
	neighborChunksID[3] = neighborID;

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
		Profiler::StartTracking("Chunk::CompileData()");
		CompileData();
		Profiler::StopTracking("Chunk::CompileData()");
		didUpdate = true;
	}
}

bool Chunk::IsGenerated() {
	return isGenerated;
}

bool Chunk::DidUpdate() {
	return didUpdate;
}

t_vertexData &Chunk::GetVertexData() {
	SetReady();
	didUpdate = false;
	return dataStruct;
}

std::vector<u_int>&		Chunk::GetShapeAssemblyData() {
	SetReady();
	didUpdate = false;
	return shapeAssemblyData;
}

Chunk::~Chunk() {
	UnloadChunk();
}