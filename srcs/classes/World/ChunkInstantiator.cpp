#include <classes/World/ChunkInstantiator.hpp>
bool isInCircle(long int x, long int y, long int radius, long int circleX, long int circleY) {
	if ((x - circleX) * (x - circleX) + (y - circleY) * (y - circleY) <= radius * radius) {
		return true;
	}
	return false;
}

ChunkInstantiator::ChunkInstantiator(VertexArrayObjectHandler *vertexArrayObjectHandler, int renderDistance, ShaderHandler *shaderHandler) {

	// updateGen("generation");

	this->shaderHandler = shaderHandler;
	Shader *shader = shaderHandler->GetShader(ChunkRLE::shaderName);
	bool showChunkDebug = false;
	this->vertexArrayObjectHandler = vertexArrayObjectHandler;
	this->renderDistance = renderDistance;
	int chunkLoadingSize = Chunk::GetLoadedChunks().size();
	std::vector<Chunk*> chunks;

	u_int seed = 12345;
	PerlinNoise *noise0 = new PerlinNoise(seed);
	PerlinNoise *noise1 = new PerlinNoise(seed + 13);

	PerlinNoise *noise2 = new PerlinNoise(seed + 59);
	PerlinNoise *noise3 = new PerlinNoise(seed + 42);
	PerlinNoise *noise4 = new PerlinNoise(seed + 53);
	PerlinNoise *noise5 = new PerlinNoise(seed + 23);
	PerlinNoise *noise6 = new PerlinNoise(seed + 17);
	PerlinNoise *noise7 = new PerlinNoise(seed + 19);

	ChunkGenerator init;
	init.pushBackNoiseList(noise0);
	init.pushBackNoiseList(noise1);
	init.pushBackNoiseList(noise2);
	init.pushBackNoiseList(noise3);
	init.pushBackNoiseList(noise4);
	init.pushBackNoiseList(noise5);
	init.pushBackNoiseList(noise6);
	init.pushBackNoiseList(noise7);


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
		VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(x->GetVertexData()), new ElementBufferObject(x->GetShapeAssemblyData()), shader);
		VAO->posX = x->GetX();
		VAO->posY = x->GetY();
		chunkMap[x] = vertexArrayObjectHandler->AddVAO(VAO);
		showChunkDebug && std::cout << "END compil " << x->GetX() << " " << x->GetY() << std::endl;
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

	if (playerChunkPosX != oldPlayerChunkPosX || playerChunkPosY != oldPlayerChunkPosY) {
		for (int x = oldPlayerChunkPosX - renderDistance; x <= oldPlayerChunkPosX + renderDistance; x++) { //Deleting chunks
			for (int y = oldPlayerChunkPosY - renderDistance; y <= oldPlayerChunkPosY + renderDistance; y++) {
				if (loadedChunks[(x % size + size) % size][(y % size + size) % size] && !isInCircle(x, y, renderDistance, playerChunkPosX, playerChunkPosY)) {
					generationQueueMap.erase(std::pair(x, y));
					compilationQueueMap.erase(std::pair(x, y));
					updateQueueMap.erase(std::pair(x, y));
					vertexArrayObjectHandler->RemoveVAO(chunkMap[loadedChunks[(x % size + size) % size][(y % size + size) % size]]);
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
	

	for (auto const& pos : generationQueueMap)
	{
		if (std::chrono::high_resolution_clock::now() - start > timeBudget) {
			for (auto const& erase : toErase) {
				generationQueueMap.erase(erase);
			}
			return ;
		}
		pos.second->PublicGenerate();
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
		VAO->posX = pos.second->GetX();
		VAO->posY = pos.second->GetY();
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
		VAO->posX = pos.second->GetX();
		VAO->posY = pos.second->GetY();
		chunkMap[pos.second] = vertexArrayObjectHandler->AddVAO(VAO);
		toErase.push_back(pos.first);
	}
	for (auto const& erase : toErase) {
		updateQueueMap.erase(erase);
	}
}

// void ChunkInstantiator::updateGen(const char *filePath)
// {
// 	std::ifstream file(filePath);

// 	if (!file.is_open())
//     {
//         std::cout << "Error opening file : " << filePath << std::endl;
//         assert(0);
//     }

//     std::string line;
    
// 	while (std::getline(file, line))
// 	{
// 		std::istringstream iss(line);
//         std::vector<std::string> tokens;

//         do
//         {
//             std::string token;
//             iss >> token;
//             if (!token.empty())
//                 tokens.push_back(token);
//         } while (iss);

//         if (tokens.size() >= 4)
//         {
//             try
//             {
// 				int seed = std::stoi(tokens[1]);
// 				std::cout << "Seed: " << seed ;
//                 this->noiseList.push_back(new PerlinNoise(seed));
// 				std::vector<double> weight;
//                 weight.push_back(std::stof(tokens[2]));
//                 weight.push_back(std::stof(tokens[3]));
//                 weight.push_back(std::stof(tokens[4]));
// 				this->weightList.push_back(weight);
// 				for (int i = 0; i < weight.size(); i++) {
// 					std::cout << "  " << weight[i];
// 				}
// 				std::cout << std::endl;

//                 // Now you can use seed, val1, val2, and val3 as needed
//                 // For example, print them
//                 // std::cout << "Seed: " << seed << ", Value 1: " << val1 << ", Value 2: " << val2 << ", Value 3: " << val3 << std::endl;
//             }
//             catch (const std::exception &e)
//             {
//                 // Handle conversion errors
//                 std::cerr << "Error parsing line: " << line << std::endl;
//             }
//         }
//         else
//         {
//             // Handle lines with insufficient tokens
//             std::cerr << "Invalid line format: " << line << std::endl;
//         }


// 	}
// }