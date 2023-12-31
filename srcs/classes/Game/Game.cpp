#include <main.hpp>

glm::vec3 const Game::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Game::Game() {
	window = new Window("ft_vox", DrawMode::FILL);

	inputHandler = new InputHandler(window->GetWindow());
	inputHandler->AddCallback((I_Input*)this);
	inputHandler->AddCallback((I_Input*)window);

	shaderHandler = new ShaderHandler("shaders");

	vertexArrayObjectHandler = new VertexArrayObjectHandler();

	Chunk::setRenderDistance(renderDistance);

	instantiator = new ChunkInstantiator(vertexArrayObjectHandler, renderDistance, shaderHandler);
}

void Game::StartLoop() {
	while(window->ShouldContinue())
	{
		Loop();
	}
}

void Game::Loop() {
	inputHandler->HandleInput();
	window->Clear();
	instantiator->Update(cameraPosition, std::chrono::milliseconds(10));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)DEFAULT_WINDOW_WIDTH/(float)DEFAULT_WINDOW_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = proj * GetCameraView();
	if (Shader::GetActiveShader()) {
		Shader::GetActiveShader()->Setmat4("matrix", matrix);
	}
	else {
		assert(!"The loop is running and there are no active shader");
	}

	vertexArrayObjectHandler->DrawAll();

	window->SwapBuffersAndPollEvents();
}

void Game::SendKeys(u_char *keyState, double mouseMoveX, double mouseMoveY) {
	float speedMultiplier = (keyState[KEY_MOVE_UPWARD] & KEY_HOLD) ? 100 : 1;
	if(keyState[KEY_MOVE_FORWARD] & KEY_HOLD)
        cameraPosition += speed * speedMultiplier * cameraDirection;
    if(keyState[KEY_MOVE_BACKWARD] & KEY_HOLD)
        cameraPosition -= speed * speedMultiplier * cameraDirection;
    if(keyState[KEY_MOVE_RIGHTWARD] & KEY_HOLD)
        cameraPosition += speed * speedMultiplier * glm::normalize(glm::cross(cameraDirection, cameraUp));
    if(keyState[KEY_MOVE_LEFTWARD] & KEY_HOLD)
        cameraPosition -= speed * speedMultiplier * glm::normalize(glm::cross(cameraDirection, cameraUp));
    if(keyState[KEY_SPACE] & KEY_HOLD)
        cameraPosition += glm::vec3(0, speed * speedMultiplier, 0);
    if(keyState[KEY_MOVE_DOWNWARD] & KEY_HOLD)
        cameraPosition += glm::vec3(0, -speed * speedMultiplier, 0);

	yaw += mouseMoveX * sensitivity;
	pitch -= mouseMoveY * sensitivity;
	if(pitch > 89.0f)
		pitch =  89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;
	cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDirection.y = sin(glm::radians(pitch));
	cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	view = glm::lookAt(	cameraPosition, 
						cameraPosition + glm::normalize(cameraDirection),
						cameraUp);
}

int Game::GetRenderDistance() const {
	return renderDistance;
}

int	Game::GetChunkLoadingSize() const {
	return chunkLoadingSize;
}

glm::mat4	Game::GetCameraView() const {
	return view;
}

Game::~Game() {}