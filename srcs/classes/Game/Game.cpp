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
	blockTexture = TextureLoader::LoadTexture("textures/minecraft.png");
	std::cout << "Game::Game()" << std::endl;
	skyBox = new SkyBox(shaderHandler->GetShader("skyBox"));

}

void Game::StartLoop() {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	u_int fps = 0;

	bool info = false;
	while(window->ShouldContinue())
	{
		fps++;
		Loop();
		if (info && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() >= 500) {
			std::cout << "chunk : " << (int)cameraPosition.x / 16 << " " << (int)cameraPosition.z / 16 << " " << (int)cameraPosition.y << std::endl;
			std::cout << "pos : " << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << std::endl;
			std::cout << "FPS: " << fps * 2 << std::endl;
			fps = 0;
			begin = std::chrono::steady_clock::now();
		}
	}
}

void Game::Loop() {
	inputHandler->HandleInput();
	window->Clear();
	instantiator->Update(cameraPosition, std::chrono::milliseconds(10));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)DEFAULT_WINDOW_WIDTH/(float)DEFAULT_WINDOW_HEIGHT, 0.1f, 16000.0f);
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = proj * GetCameraView();
	if (Shader::GetActiveShader()) {
		Shader::GetActiveShader()->Setmat4("matrix", matrix);
	}
	else {
		assert(!"The loop is running and there are no active shader");
	}
	
	


	skyBox->drawSkybox(matrix, cameraPosition);




	glBindTexture(GL_TEXTURE_2D, blockTexture.id);
	vertexArrayObjectHandler->DrawAll();
	window->SwapBuffersAndPollEvents();

}

bool	Game::putBlock(glm::vec3 pos, u_char type) {
	int chunkX = pos.x / 16;
	int chunkY = pos.z / 16;
	int blockX = abs(pos.x - chunkX * 16);
	int blockY = abs(pos.z - chunkY * 16);
	int blockZ = pos.y;
	if (blockX < 0 || blockY < 0 || blockX >= 16 || blockY >= 16 || blockZ < 0 || blockZ >= 256)
		return false;
	std::cout << "in" << std::endl;
	for (int i = 0; i < Chunk::loadedChunks.size(); i++) {
		for (int j = 0; j < Chunk::loadedChunks[i].size(); j++) {
			if (Chunk::loadedChunks[i][j] && Chunk::loadedChunks[i][j]->GetX() == chunkX && Chunk::loadedChunks[i][j]->GetY() == chunkY
				&& Chunk::loadedChunks[i][j]->isFilled(blockX, blockY, blockZ)) {
				
				std::vector<u_char> *modif = ChunkGenerator::modifMap[position(chunkX, chunkY)];
				if (!modif) {
					modif = new std::vector<u_char>();
					ChunkGenerator::modifMap[position(chunkX, chunkY)] = modif;
				}
				modif->push_back(blockX);
				modif->push_back(blockY);
				modif->push_back(blockZ);
				modif->push_back(type);
				Chunk::loadedChunks[i][j]->MakeDirty();
				std::cout << "pos put: " << pos.x << " " << pos.y << " " << pos.z << std::endl;
				std::cout << "------putBlock" << std::endl;
				return true;
			}
		}
	}
	return false;
}

void Game::deleteBlock()
{
	glm::vec3 direction = glm::normalize(cameraDirection);
	glm::vec3 position = cameraPosition;
	
	glm::vec3 toFind = position;
	glm::vec3 incr = glm::vec3(-1, -1, -1);
	if (direction.x > 0)
		incr.x = 1;
	if (direction.y > 0)
		incr.y = 1;
	if (direction.z > 0)
		incr.z = 1;

	int i = 0;
	while (i < 30 && (position - toFind).length() < 10 && (position - toFind).length() > -10)
	{
		i++;
		glm::vec3 steps = {0, 0, 0};
		glm::vec3 distance = {0, 0, 0};
		

		for (int j = 0; j < 3; j++)
		{
			if (direction[j] < 0)
				distance[j] = abs(ceil(toFind[j])) - abs(toFind[j]);
			else
				distance[j] = abs(toFind[j]) - abs(floor(toFind[j]));
		}

		steps.x = abs(distance.x / direction.x);
		steps.y = abs(distance.y / direction.y);
		steps.z = abs(distance.z / direction.z);


		if (steps.x < steps.y && steps.x < steps.z)
		{
			std::cout << "x" << std::endl;
			toFind += steps.x * direction;
			if (putBlock(toFind, AIR))
				return;
		}
		else if (steps.y < steps.x && steps.y < steps.z)
		{
			std::cout << "y" << std::endl;
			toFind += steps.y * direction;
			if (putBlock(toFind, AIR))
				return;
		}
		else
		{
			std::cout << "z" << std::endl;
			toFind += steps.z * direction;
			if (putBlock(toFind, AIR))
				return;
		}
	}
	std::cout << "Not put" << std::endl;
}

void Game::SendKeys(u_char *keyState, double mouseMoveX, double mouseMoveY) {
	float speedMultiplier = (keyState[KEY_MOVE_UPWARD] & KEY_HOLD) ? 20 : 1;
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
	// if (glfwGetKey(window->GetWindow(), GLFW_KEY_G) == GLFW_PRESS) {
	// 	instantiator->updateGen("generation");
	// }
	if(keyState[KEY_DELETE_BLOCK] & KEY_PRESS)
        deleteBlock();

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

// glm::vec3 	Game::GetCameraPosition() const
// {
// 	return cameraPosition;
// }

Game::~Game() {}