#include <classes/World/ChunkInstantiator.hpp>

ChunkInstantiator::ChunkInstantiator(VertexArrayObjectHandler *vertexArrayObjectHandler, int renderDistance) {
	this->vertexArrayObjectHandler = vertexArrayObjectHandler;
	this->renderDistance = renderDistance;
	int chunkLoadingSize = Chunk::GetLoadedChunks().size();
	std::vector<Chunk*> chunks;

	std::cout << "Chunk generation started " << std::endl;
	for (int x = -renderDistance; x <= renderDistance; x++) { // GENERER TOUT PUIS COMPILER
		for (int y = -renderDistance; y <= renderDistance; y++) {
			std::cout << "Generating chunk " << x << " " << y << std::endl;
			Chunk* chunk = new ChunkDefault(x, y);
			chunk->PublicGenerate();
			chunks.push_back(chunk);
		}
	}
	std::cout << "Chunk are generated " << std::endl;

	std::cout << "Chunk compilation started " << std::endl;
	int i = 0;
	for (int x = -renderDistance; x <= renderDistance; x++) {
		for (int y = -renderDistance; y <= renderDistance; y++) {
			std::cout << "Compiling chunk " << x << " " << y << std::endl;
			VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(chunks[i]->GetVertexData()), new ElementBufferObject(chunks[i]->GetShapeAssemblyData()));
			chunkMap[chunks[i]] = vertexArrayObjectHandler->AddVAO(VAO);
			VAO->AddVertexAttribute(0, 3, 1.0f);
			vertexArrayObjectHandler->Unbind();
			i++;
		}
	}
	std::cout << "Chunk are compiled " << std::endl;
}

void ChunkInstantiator::Update(glm::vec3 playerPos) {
	const std::vector<std::vector<Chunk *>> &loadedChunks = Chunk::GetLoadedChunks();
	int size = loadedChunks.size();
	playerPos /= 16;

	int newPlayerChunkPosX = playerPos.x;
	int newPlayerChunkPosY = playerPos.z;

	if (newPlayerChunkPosX != playerChunkPosX) {
		int X;
		if (newPlayerChunkPosX > playerChunkPosX) { 
			X = newPlayerChunkPosX + renderDistance;
		}
		else {
			X = newPlayerChunkPosX - renderDistance;
		}
		std::vector<Chunk*> chunks;
		for (int y = playerChunkPosY - renderDistance; y <= playerChunkPosY + renderDistance; y++) {
			vertexArrayObjectHandler->RemoveVAO(chunkMap[loadedChunks[(X % size + size) % size][(y % size + size) % size]]);
			chunkMap.erase(loadedChunks[(X % size + size) % size][(y % size + size) % size]);
			delete loadedChunks[(X % size + size) % size][(y % size + size) % size];
			Chunk* chunk = new ChunkDefault(X, y);
			chunk->PublicGenerate();
			chunks.push_back(chunk);
		}
		int i = 0;
		vertexArrayObjectHandler->Unbind();
		for (auto const& x : chunks)
		{
			VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(x->GetVertexData()), new ElementBufferObject(x->GetShapeAssemblyData()));
			chunkMap[x] = vertexArrayObjectHandler->AddVAO(VAO);
			VAO->AddVertexAttribute(0, 3, 1.0f);
			vertexArrayObjectHandler->Unbind();
			i++;
		}
	}
	if (newPlayerChunkPosY != playerChunkPosY) {
		int Y;
		if (newPlayerChunkPosY > playerChunkPosY) { 
			Y = newPlayerChunkPosY + renderDistance;
		}
		else {
			Y = newPlayerChunkPosY - renderDistance;
		}
		std::vector<Chunk*> chunks;
		for (int x = newPlayerChunkPosX - renderDistance; x <= newPlayerChunkPosX + renderDistance; x++) {
			vertexArrayObjectHandler->RemoveVAO(chunkMap[loadedChunks[(x % size + size) % size][(Y % size + size) % size]]);
			chunkMap.erase(loadedChunks[(x % size + size) % size][(Y % size + size) % size]);
			delete loadedChunks[(x % size + size) % size][(Y % size + size) % size];
			Chunk* chunk = new ChunkDefault(x, Y);
			chunk->PublicGenerate();
			chunks.push_back(chunk);
		}
		int i = 0;
		vertexArrayObjectHandler->Unbind();
		for (auto const& x : chunks)
		{
			VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(x->GetVertexData()), new ElementBufferObject(x->GetShapeAssemblyData()));
			chunkMap[x] = vertexArrayObjectHandler->AddVAO(VAO);
			VAO->AddVertexAttribute(0, 3, 1.0f);
			vertexArrayObjectHandler->Unbind();
			i++;
		}
	}

	vertexArrayObjectHandler->Unbind();
	std::vector<Chunk*> temp;
	int i = 0;
	for (auto const& x : chunkMap)
	{
		i++;
		if (x.first->DidUpdate()) {
			temp.push_back(x.first);
		}
	}
	for (auto const& x : temp) {
		
		vertexArrayObjectHandler->RemoveVAO(chunkMap[x]);
		chunkMap.erase(x);
		VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(x->GetVertexData()), new ElementBufferObject(x->GetShapeAssemblyData()));
		chunkMap[x] = vertexArrayObjectHandler->AddVAO(VAO);
		VAO->AddVertexAttribute(0, 3, 1.0f);
		vertexArrayObjectHandler->Unbind();
	}

	playerChunkPosX = newPlayerChunkPosX;
	playerChunkPosY = newPlayerChunkPosY;
}