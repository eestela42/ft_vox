#include <main.hpp>

int main(int argc, char **argv) {
	Window *window = new Window("vox", DrawMode::FILL);

	InputHandler *inputHandler = new InputHandler(window->GetWindow());
	Game *game = new Game();
	inputHandler->AddCallback((I_Input*)game);
	inputHandler->AddCallback((I_Input*)window);

	Shader *shader = new Shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
	shader->Use();

	VertexArrayObjectHandler *vertexArrayObjectHandler = new VertexArrayObjectHandler();

	while(window->ShouldContinue())
	{
		inputHandler->HandleInput();
		window->Clear();

		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)DEFAULT_WINDOW_WIDTH/(float)DEFAULT_WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = proj * glm::translate(matrix, game->GetCameraPosition()) * glm::toMat4(game->GetCameraQuaternion());
		shader->Setmat4("matrix", matrix);

		vertexArrayObjectHandler->Draw();
		window->SwapBuffersAndPollEvents();
	}
	return 0;
}