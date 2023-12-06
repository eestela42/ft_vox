#include <main.hpp>

int main(int argc, char **argv) {
	Window *window = new Window("ft_vox", DrawMode::FILL);

	InputHandler *inputHandler = new InputHandler(window->GetWindow());
	Game *game = new Game();
	inputHandler->AddCallback((I_Input*)game);
	inputHandler->AddCallback((I_Input*)window);

	ShaderHandler *shaderHandler = new ShaderHandler("shaders");
	Shader *shader = shaderHandler->GetShader("default_shaders");
	shader->Use();

	VertexArrayObjectHandler *vertexArrayObjectHandler = new VertexArrayObjectHandler();
	Chunk chunk;
	VertexArrayObject *VAO_1 = new VertexArrayObject(new VertexBufferObject(chunk.GetVertexData()), new ElementBufferObject(chunk.GetShapeAssemblyData()));
	u_int vao_1 = vertexArrayObjectHandler->AddVAO(VAO_1);
	VAO_1->AddVertexAttribute(0, 3, 1.0f);

	vertexArrayObjectHandler->Bind(vao_1);


	while(window->ShouldContinue())
	{
		inputHandler->HandleInput();
		window->Clear();

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)DEFAULT_WINDOW_WIDTH/(float)DEFAULT_WINDOW_HEIGHT, 0.1f, 250.0f);
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = proj * glm::translate(matrix, game->GetCameraPosition()) * glm::toMat4(game->GetCameraQuaternion());
		shader->Setmat4("matrix", matrix);

		vertexArrayObjectHandler->Draw();
		window->SwapBuffersAndPollEvents();
	}
	return 0;
}