#include <classes/World/ChunkInstantiator.hpp>
#include <classes/Game/Game.hpp>
bool isInCircle(long int x, long int y, long int radius, long int circleX, long int circleY) {
	if ((x - circleX) * (x - circleX) + (y - circleY) * (y - circleY) <= radius * radius) {
		return true;
	}
	return false;
}

ChunkInstantiator::~ChunkInstantiator()
{
	const std::vector<std::vector<Chunk *>> &loadedChunks = Chunk::GetLoadedChunks();

	for (unsigned long i = 0; i  < loadedChunks.size(); i++)
	{
		for (unsigned long y = 0; y < loadedChunks[i].size(); y++)
		{
			delete loadedChunks[i][y];
		}
	}

	
}


ChunkInstantiator::ChunkInstantiator(VertexArrayObjectHandler *vertexArrayObjectHandler, int renderDistance, ShaderHandler *shaderHandler) {

	// updateGen("generation");

	this->shaderHandler = shaderHandler;

	playerChunkPosX = 0;
	playerChunkPosY = 0;
	Shader *shader = shaderHandler->GetShader(ChunkRLE::shaderName);
	bool showChunkDebug = false;
	this->vertexArrayObjectHandler = vertexArrayObjectHandler;
	this->renderDistance = renderDistance;
	// int chunkLoadingSize = Chunk::GetLoadedChunks().size();
	std::vector<Chunk*> chunks;

	//get an random long int 

	std::srand(std::time(0));
	long int seed;
	//1809831956 is problem
	seed = 1809831956;
	seed = std::rand();
	std::cout << ChunkRLE::shaderName << std::endl;
	std::cout << "seed " << seed << std::endl;
	ChunkGenerator::initNoise(seed);


	showChunkDebug && std::cout << "Chunk generation started " << std::endl;
	for (int x = -renderDistance; x <= renderDistance; x++) { //Creating chunks
	for (int y = -renderDistance; y <= renderDistance; y++) {
		if (isInCircle(x, y, renderDistance, 0, 0)) {
			showChunkDebug && std::cout << "Generating chunk " << x << " " << y << std::endl;
			Chunk* chunk = new ChunkRLE(x, y);
			chunk->PublicGenerate();


			chunks.push_back(chunk);

		}
	}}
	showChunkDebug && std::cout << "Chunk are generated " << std::endl;

	showChunkDebug && std::cout << "Chunk compilation started " << std::endl;
	for (auto const& x : chunks)
	{
		showChunkDebug && std::cout << "compil " << x->GetX() << " " << x->GetY() << std::endl;
		// VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(x->GetVertexData()), new ElementBufferObject(x->GetShapeAssemblyData()), shader);
		// VAO->posX = x->GetX();
		// VAO->posY = x->GetY();
		// chunkMap[x] = vertexArrayObjectHandler->AddVAO(VAO);
		chunkMap[x] = vertexArrayObjectHandler->addVaoCreateQueue(&x->GetVertexData(), &x->GetShapeAssemblyData());

		showChunkDebug && std::cout << "END compil " << x->GetX() << " " << x->GetY() << std::endl;
	}
	showChunkDebug && std::cout << "Chunk are compiled " << std::endl;
}

void ChunkInstantiator::Update(glm::vec3 playerPos) {
	std::chrono::_V2::system_clock::time_point start = std::chrono::high_resolution_clock::now();
	Shader *shader = shaderHandler->GetShader(ChunkRLE::shaderName);
	const std::vector<std::vector<Chunk *>> &loadedChunks = Chunk::GetLoadedChunks();
	int size = loadedChunks.size();
	playerPos.x /= Chunk::sizeX;//ChunkSize
	playerPos.z /= Chunk::sizeY;

	int oldPlayerChunkPosX = playerChunkPosX;
	int oldPlayerChunkPosY = playerChunkPosY;
	playerChunkPosX = playerPos.x;
	playerChunkPosY = playerPos.z;
	std::cout << "1" << std::endl;
	if (playerChunkPosX != oldPlayerChunkPosX || playerChunkPosY != oldPlayerChunkPosY) {
		for (int x = oldPlayerChunkPosX - renderDistance; x <= oldPlayerChunkPosX + renderDistance; x++) { //Deleting chunks
			for (int y = oldPlayerChunkPosY - renderDistance; y <= oldPlayerChunkPosY + renderDistance; y++) {
				if (loadedChunks[(x % size + size) % size][(y % size + size) % size] && !isInCircle(x, y, renderDistance, playerChunkPosX, playerChunkPosY)) {
					generationQueueMap.erase(std::pair(x, y));
					compilationQueueMap.erase(std::pair(x, y));
					updateQueueMap.erase(std::pair(x, y));
					vertexArrayObjectHandler->addVaoDeleteQueue(chunkMap[loadedChunks[(x % size + size) % size][(y % size + size) % size]]);
					// vertexArrayObjectHandler->RemoveVAO(chunkMap[loadedChunks[(x % size + size) % size][(y % size + size) % size]]);
					chunkMap.erase(loadedChunks[(x % size + size) % size][(y % size + size) % size]);
					delete loadedChunks[(x % size + size) % size][(y % size + size) % size];
				}
			}
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
			}
		}
	}
	std::vector<std::pair<int,int>> toErase;
	
	std::cout << "2" << std::endl;

	for (auto const& pos : generationQueueMap)
	{
			for (auto const& erase : toErase) {
				generationQueueMap.erase(erase);
		}
		std::cout << "2.1" << std::endl;
		pos.second->PublicGenerate();
		std::cout << "2.2" << std::endl;
		toErase.push_back(pos.first);

	}
	for (auto const& erase : toErase) {
		generationQueueMap.erase(erase);
	}
	std::cout << "3" << std::endl;

	toErase.clear();
	for (auto const& pos : compilationQueueMap)
	{
			for (auto const& erase : toErase) {
				compilationQueueMap.erase(erase);
			}
		// VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(pos.second->GetVertexData()), new ElementBufferObject(pos.second->GetShapeAssemblyData()), shader);
		// VAO->posX = pos.second->GetX();
		// VAO->posY = pos.second->GetY();
		// chunkMap[pos.second] = vertexArrayObjectHandler->AddVAO(VAO);
		std::cout << "3.1" << std::endl;
		chunkMap[pos.second] = vertexArrayObjectHandler->addVaoCreateQueue(&pos.second->GetVertexData(), &pos.second->GetShapeAssemblyData());
		std::cout << "3.2" << std::endl;
		
		toErase.push_back(pos.first);
	}
	for (auto const& erase : toErase) {
		compilationQueueMap.erase(erase);
	}
	std::cout << "4" << std::endl;

	for (auto const& x : chunkMap)
	{
		if (x.first->DidUpdate()) {
			if (updateQueueMap.find(std::pair(x.first->GetX(), x.first->GetY())) == generationQueueMap.end()) {
				updateQueueMap[std::pair(x.first->GetX(), x.first->GetY())] = x.first;
			}
		}
	}
	std::cout << "5" << std::endl;

	toErase.clear();
	for (auto const& pos : updateQueueMap) {
			for (auto const& erase : toErase) {
				updateQueueMap.erase(erase);
			}
		// vertexArrayObjectHandler->RemoveVAO(chunkMap[pos.second]);
		vertexArrayObjectHandler->addVaoDeleteQueue(chunkMap[pos.second]);
		chunkMap.erase(pos.second);
		// VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(pos.second->GetVertexData()), new ElementBufferObject(pos.second->GetShapeAssemblyData()), shader);
		// VAO->posX = pos.second->GetX();
		// VAO->posY = pos.second->GetY();
		// chunkMap[pos.second] = vertexArrayObjectHandler->AddVAO(VAO);
		chunkMap[pos.second] = vertexArrayObjectHandler->addVaoCreateQueue(&pos.second->GetVertexData(), &pos.second->GetShapeAssemblyData());
		toErase.push_back(pos.first);
	}
	for (auto const& erase : toErase) {
		updateQueueMap.erase(erase);
	}
}