#include <main.hpp>

int main(int argc, char **argv) {

	Window *window = new Window("ft_vox", DrawMode::FILL);

	InputHandler *inputHandler = new InputHandler(window->GetWindow());
	Game *game = new Game();
	inputHandler->AddCallback((I_Input*)game);
	inputHandler->AddCallback((I_Input*)window);

	ShaderHandler *shaderHandler = new ShaderHandler("shaders");
	Shader *shader = shaderHandler->GetShader(ChunkRLE::shaderName);
	shader->Use();

	VertexArrayObjectHandler *vertexArrayObjectHandler = new VertexArrayObjectHandler();
	std::vector<u_int> chunkMap;

	Chunk::setRenderDistance(game->GetRenderDistance());


	int size = game->GetRenderDistance() * 2 + 1;
	std::vector<Chunk*> chunks;

	// Record the start time
    auto start_time = std::chrono::high_resolution_clock::now();

    // Your code to measure goes here

    // Record the end time
    

    // Print the start and end times
    
    // Print the elapsed time in microseconds

	Texture textureHandler = TextureLoader::LoadTexture("textures/minecraft.png");

  	u_int seed = 16254;

	PerlinNoise *noise = new PerlinNoise(seed);

	PerlinNoise *noise2 = new PerlinNoise(seed + 1358);

	for (int x = 0; x < size; x++) { // GENERER TOUT PUIS COMPILER
		for (int y = 0; y < size; y++) {
			ChunkRLE* chunk = new ChunkRLE(x, y);
			chunk->PublicGenerateTest(noise, noise2);
			chunks.push_back(chunk);
		}
	}

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			// std::vector<int> *data = new std::vector<int>;
			// data->resize(chunks[x + y * size]->GetVertexData().size());
			// for (int i = 0; i < chunks[x + y * size]->GetVertexData().size(); i++)
			// {
			// 	std::memcpy(&data->at(i), &chunks[x + y * size]->GetVertexData()[i], sizeof(int));
			// }

			VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(chunks[x + y * size]->GetVertexData()), new ElementBufferObject(chunks[x + y * size]->GetShapeAssemblyData()));
			chunkMap.push_back(vertexArrayObjectHandler->AddVAO(VAO));
			VAO->AddVertexAttribute(0, 1, (int)1);
			VAO->AddVertexAttribute(1, 1, (int)1);
			VAO->AddVertexAttribute(2, 1, (int)1);
			VAO->AddVertexAttribute(3, 1, (int)1);
			VAO->AddVertexAttribute(4, 1, (int)1);
			vertexArrayObjectHandler->Unbind();
		}
	}

	auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the elapsed time
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

	std::cout << "Elapsed Time: " << duration.count()/1000 << " milliseconds" << std::endl;



	while(window->ShouldContinue())
	{
		inputHandler->HandleInput();
		window->Clear();
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)DEFAULT_WINDOW_WIDTH/(float)DEFAULT_WINDOW_HEIGHT, 0.1f, 1250.0f);
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = proj * game->GetCameraView();
		shader->Setmat4("matrix", matrix);
		for (auto const& x : chunkMap)
		{
			vertexArrayObjectHandler->Bind(x);
			vertexArrayObjectHandler->Draw();
		}
		window->SwapBuffersAndPollEvents();
	}
	return 0;


}