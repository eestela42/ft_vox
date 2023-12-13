#include <classes/World/ChunkInstantiator.hpp>

bool isInCircle(long int x, long int y, long int radius, long int circleX, long int circleY) {
	if ((x - circleX) * (x - circleX) + (y - circleY) * (y - circleY) <= radius * radius) {
		return true;
	}
	return false;
}

ChunkInstantiator::ChunkInstantiator(VertexArrayObjectHandler *vertexArrayObjectHandler, int renderDistance, ShaderHandler *shaderHandler) {
	this->shaderHandler = shaderHandler;
	Shader *shader = shaderHandler->GetShader(ChunkRLE::shaderName);
	bool showChunkDebug = false;
	this->vertexArrayObjectHandler = vertexArrayObjectHandler;
	this->renderDistance = renderDistance;
	int chunkLoadingSize = Chunk::GetLoadedChunks().size();
	std::vector<Chunk*> chunks;

	u_int seed = 32543;
	PerlinNoise *noise = new PerlinNoise(seed);
	PerlinNoise *noise2 = new PerlinNoise(seed + 13);

	showChunkDebug && std::cout << "Chunk generation started " << std::endl;
	for (int x = -renderDistance; x <= renderDistance; x++) { //Creating chunks
	for (int y = -renderDistance; y <= renderDistance; y++) {
		if (isInCircle(x, y, renderDistance, 0, 0)) {
			showChunkDebug && std::cout << "Generating chunk " << x << " " << y << std::endl;
			
			Chunk* chunk = new ChunkRLE(x, y);
			chunk->PublicGenerate(noise, noise2);


			chunks.push_back(chunk);
		}
	}}
	showChunkDebug && std::cout << "Chunk are generated " << std::endl;

	showChunkDebug && std::cout << "Chunk compilation started " << std::endl;
	for (auto const& x : chunks)
	{
		VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(x->GetVertexData()), new ElementBufferObject(x->GetShapeAssemblyData()), shader);
		chunkMap[x] = vertexArrayObjectHandler->AddVAO(VAO);
	}
	showChunkDebug && std::cout << "Chunk are compiled " << std::endl;
}

void ChunkInstantiator::Update(glm::vec3 playerPos, std::chrono::milliseconds timeBudget) {
	std::chrono::_V2::system_clock::time_point start = std::chrono::high_resolution_clock::now();
	Shader *shader = shaderHandler->GetShader(ChunkRLE::shaderName);
	const std::vector<std::vector<Chunk *>> &loadedChunks = Chunk::GetLoadedChunks();
	int size = loadedChunks.size();

	playerPos /= 16;//ChunkSize

	int oldPlayerChunkPosX = playerChunkPosX;
	int oldPlayerChunkPosY = playerChunkPosY;
	playerChunkPosX = playerPos.x;
	playerChunkPosY = playerPos.z;

	std::cout << generationQueueMap.size() << " " << compilationQueueMap.size() << " " << updateQueueMap.size() << std::endl;
	if (playerChunkPosX != oldPlayerChunkPosX || playerChunkPosY != oldPlayerChunkPosY) {
		for (int x = oldPlayerChunkPosX - renderDistance; x <= oldPlayerChunkPosX + renderDistance; x++) { //Deleting chunks
		for (int y = oldPlayerChunkPosY - renderDistance; y <= oldPlayerChunkPosY + renderDistance; y++) {
			if (loadedChunks[(x % size + size) % size][(y % size + size) % size] && !isInCircle(x, y, renderDistance, playerChunkPosX, playerChunkPosY)) {
				std::cout << "boucle " << std::endl;
				generationQueueMap.erase(std::pair(x, y));
				compilationQueueMap.erase(std::pair(x, y));
				updateQueueMap.erase(std::pair(x, y));
				vertexArrayObjectHandler->RemoveVAO(chunkMap[loadedChunks[(x % size + size) % size][(y % size + size) % size]]);
				chunkMap.erase(loadedChunks[(x % size + size) % size][(y % size + size) % size]);
				std::cout << "in inst before delete chunk " << x << " " << y << std::endl;
				delete loadedChunks[(x % size + size) % size][(y % size + size) % size];
				std::cout << "in inst after delete" << std::endl;
			}
		}
		std::cout << "in inst out boucle" << std::endl;
		}
		for (int x = playerChunkPosX - renderDistance; x <= playerChunkPosX + renderDistance; x++) { //Creating chunks
		for (int y = playerChunkPosY - renderDistance; y <= playerChunkPosY + renderDistance; y++) {
			if (isInCircle(x, y, renderDistance, playerChunkPosX, playerChunkPosY) && !isInCircle(x, y, renderDistance, oldPlayerChunkPosX, oldPlayerChunkPosY)) {
				if (generationQueueMap.find(std::pair(x, y)) == generationQueueMap.end()) {
					Chunk* chunk = new ChunkRLE(x, y);
					generationQueueMap[std::pair(x, y)] = chunk;
					compilationQueueMap[std::pair(x, y)] = chunk;
				}
			}
		}}
	}
	std::vector<std::pair<int,int>> toErase;
	u_int seed = 32543;
	PerlinNoise *noise = new PerlinNoise(seed);
	PerlinNoise *noise2 = new PerlinNoise(seed + 13);

	for (auto const& pos : generationQueueMap)
	{
		if (std::chrono::high_resolution_clock::now() - start > timeBudget) {
			for (auto const& erase : toErase) {
				generationQueueMap.erase(erase);
			}
			return ;
		}
		pos.second->PublicGenerate(noise, noise2);
		toErase.push_back(pos.first);
	}
	for (auto const& erase : toErase) {
		generationQueueMap.erase(erase);
	}

	toErase.clear();
	for (auto const& pos : compilationQueueMap)
	{
		if (std::chrono::high_resolution_clock::now() - start > timeBudget) {
			for (auto const& erase : toErase) {
				compilationQueueMap.erase(erase);
			}
			return ;
		}
		VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(pos.second->GetVertexData()), new ElementBufferObject(pos.second->GetShapeAssemblyData()), shader);
		chunkMap[pos.second] = vertexArrayObjectHandler->AddVAO(VAO);
		toErase.push_back(pos.first);
	}
	for (auto const& erase : toErase) {
		compilationQueueMap.erase(erase);
	}

	for (auto const& x : chunkMap)
	{
		if (x.first->DidUpdate()) {
			if (updateQueueMap.find(std::pair(x.first->GetX(), x.first->GetY())) == generationQueueMap.end()) {
				updateQueueMap[std::pair(x.first->GetX(), x.first->GetY())] = x.first;
			}
		}
	}

	toErase.clear();
	for (auto const& pos : updateQueueMap) {
		if (std::chrono::high_resolution_clock::now() - start > timeBudget) {
			for (auto const& erase : toErase) {
				updateQueueMap.erase(erase);
			}
			return ;
		}
		vertexArrayObjectHandler->RemoveVAO(chunkMap[pos.second]);
		chunkMap.erase(pos.second);
		VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(pos.second->GetVertexData()), new ElementBufferObject(pos.second->GetShapeAssemblyData()), shader);
		chunkMap[pos.second] = vertexArrayObjectHandler->AddVAO(VAO);
		toErase.push_back(pos.first);
	}
	for (auto const& erase : toErase) {
		updateQueueMap.erase(erase);
	}
}