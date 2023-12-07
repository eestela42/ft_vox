#include <main.hpp>

int main(int argc, char **argv) {
	Window *window = new Window("ft_vox", DrawMode::FILL);

	InputHandler *inputHandler = new InputHandler(window->GetWindow());
	Game *game = new Game();
	inputHandler->AddCallback((I_Input*)game);
	inputHandler->AddCallback((I_Input*)window);

	ShaderHandler *shaderHandler = new ShaderHandler("shaders");
	Shader *shader = shaderHandler->GetShader(Chunk::shaderName);
	shader->Use();

	VertexArrayObjectHandler *vertexArrayObjectHandler = new VertexArrayObjectHandler();
	std::vector<u_int> chunkMap;
	for (int x = 0; x < 24; x++) {
		for (int y = 0; y < 24; y++) {
			std::cout << "Generating chunk " << x << " " << y << std::endl;
			ChunkRLE *chunk = new ChunkRLE(x, y);
			chunk->Generate();
			chunk->CompileData();
			VertexArrayObject *VAO = new VertexArrayObject(new VertexBufferObject(chunk->GetVertexData()), new ElementBufferObject(chunk->GetShapeAssemblyData()));
			chunkMap.push_back(vertexArrayObjectHandler->AddVAO(VAO));
			VAO->AddVertexAttribute(0, 3, 1.0f);
		}
	}

	while(window->ShouldContinue())
	{
		inputHandler->HandleInput();
		window->Clear();

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)DEFAULT_WINDOW_WIDTH/(float)DEFAULT_WINDOW_HEIGHT, 0.1f, 250.0f);
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = proj * glm::translate(matrix, game->GetCameraPosition()) * glm::toMat4(game->GetCameraQuaternion());
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