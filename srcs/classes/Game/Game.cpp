#include <main.hpp>

glm::vec3 const Game::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);




Game::~Game() {
	
	for (size_t i = 0; i < ChunkGenerator::noiseList.size(); i++)
		delete  ChunkGenerator::noiseList[i];
	
	for (auto  x : ChunkGenerator::modifMap)
	{
		delete x.second;
	}

	delete crossHair;
	delete skyBox;
	delete instantiator;
	delete vertexArrayObjectHandler;
	delete shaderHandler;
	delete inputHandler;
	delete window;


}


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

	

	blockTextureArray = TextureLoader::LoadTextureArray(
	{
	std::filesystem::path("textures/texturePack/dirt.jpg"),
	std::filesystem::path("textures/texturePack/dirt.jpg"),
	std::filesystem::path("textures/texturePack/grass.jpg"),
	std::filesystem::path("textures/texturePack/stone.jpg"),
	std::filesystem::path("textures/texturePack/sand.jpg"),
	std::filesystem::path("textures/texturePack/oak_wood_side.jpg"),
	std::filesystem::path("textures/texturePack/leaves_2.jpg"),
	std::filesystem::path("textures/texturePack/bedrock.jpg"),
	std::filesystem::path("textures/texturePack/water.jpg"),
	std::filesystem::path("textures/texturePack/snow.jpg"),
	std::filesystem::path("textures/texturePack/iron_ore.jpg"),
	std::filesystem::path("textures/texturePack/gold_ore.jpg"),
	std::filesystem::path("textures/texturePack/diamond_ore.jpg"),
	std::filesystem::path("textures/texturePack/UNKNOWN.jpg"),
	});
	skyBox = new SkyBox(shaderHandler->GetShader("cubemap"));
	crossHair = new CrossHair(shaderHandler->GetShader("CrossHair"));

	Entity::entities.push_back(new Entity(glm::vec3(100, 255, 100), shaderHandler));
	Entity::entities.push_back(new Entity(glm::vec3(80, 255, 100), shaderHandler));
	Entity::entities.push_back(new Entity(glm::vec3(100, 255, 80), shaderHandler));

}

void Game::generationLoop() {
	while (!isEnd)
		instantiator->Update(my_player.cameraPosition);
}

void Game::StartLoop() {
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	u_int fps = 0;

	bool info = true;
	std::thread generationThread(&Game::generationLoop, this);
	// generationThread.join();

	while(window->ShouldContinue())
	{
		fps++;
		Loop();
		if (info && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() >= 500) {
			// std::cout << "chunk : " << (int)cameraPosition.x / Chunk::sizeX << " " << (int)cameraPosition.z / Chunk::sizeY << " " << (int)cameraPosition.y << std::endl;
			// std::cout << "pos : " << cameraPosition.x << " " << cameraPosition.y << " " << cameraPosition.z << std::endl;
			std::cout << "FPS: " << fps * 2 << std::endl;
			fps = 0;
			begin = std::chrono::steady_clock::now();
		}
	}
	isEnd = true;
}



void Game::Loop() {
	my_player.Gravity();
	vertexArrayObjectHandler->vaoCreate(shaderHandler);
	vertexArrayObjectHandler->vaoDelete();
	for (int i = 0; i < Entity::entities.size(); i++)
	{
		Entity::entities[i]->Gravity();
		Entity::entities[i]->Movement(my_player.cameraPosition);
	}
	inputHandler->HandleInput();
	window->Clear();
	// instantiator->Update(my_player.cameraPosition, std::chrono::milliseconds(20));

	glm::mat4 view = glm::lookAt(glm::vec3(0,0,0) , my_player.cameraDirection, cameraUp);
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)DEFAULT_WINDOW_WIDTH/(float)DEFAULT_WINDOW_HEIGHT, 0.1f, 16000.0f);
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = proj * view;
	glm::normalize(my_player.cameraDirection);
	if (Shader::GetActiveShader()) {
		Shader::GetActiveShader()->SetFloat4("cameraPos", my_player.cameraPosition.x, my_player.cameraPosition.y, my_player.cameraPosition.z, 0);
		Shader::GetActiveShader()->SetFloat4("cameraDir", my_player.cameraDirection.x, my_player.cameraDirection.y, my_player.cameraDirection.z, 0);
		Shader::GetActiveShader()->Setmat4("matrix", matrix);
		Shader::GetActiveShader()->SetInt("chunk_size_x", Chunk::sizeX);
		Shader::GetActiveShader()->SetInt("chunk_size_y", Chunk::sizeY);
		Shader::GetActiveShader()->SetInt("TextureArraySize", 6);
	}
	else {
		assert(!"The loop is running and there are no active shader");
	}
	
	


	skyBox->drawSkybox(proj * glm::mat4(glm::mat3(view)), my_player.cameraPosition);

	crossHair->draw();

	glBindTexture(GL_TEXTURE_2D_ARRAY, blockTextureArray.id);

	u_int drawType = 0;
	if (ChunkRLE::shaderName == (char*)"RLE-Regular")
		drawType = 0;
	else if (ChunkRLE::shaderName == (char*)"RLE-Geometry")
		drawType = 1;

	for (auto x : Entity::entities)
		x->draw(matrix, my_player.cameraPosition);

	vertexArrayObjectHandler->DrawAll(drawType);

	window->SwapBuffersAndPollEvents();

}


void Game::SendKeys(u_char *keyState, double mouseMoveX, double mouseMoveY) {
	my_player.speedMultiplier = (keyState[KEY_MOVE_UPWARD] & KEY_HOLD) ? 20.0f : 1.0f;
	if(keyState[KEY_MOVE_FORWARD] & KEY_HOLD)
  		my_player.PlayerMove(KEY_MOVE_FORWARD);
	if(keyState[KEY_MOVE_BACKWARD] & KEY_HOLD)
		my_player.PlayerMove(KEY_MOVE_BACKWARD);
	if(keyState[KEY_MOVE_RIGHTWARD] & KEY_HOLD)
		my_player.PlayerMove(KEY_MOVE_RIGHTWARD);
	if(keyState[KEY_MOVE_LEFTWARD] & KEY_HOLD)
		my_player.PlayerMove(KEY_MOVE_LEFTWARD);
	
	if (keyState[KEY_FLY_MODE] & KEY_PRESS)
		my_player.flyMode = !my_player.flyMode;

	if(keyState[KEY_SPACE] & KEY_HOLD)
			my_player.Jump();
	if(keyState[KEY_MOVE_DOWNWARD] & KEY_HOLD)
			my_player.Sneak();
		
	
	// if(keyState[KEY_DELETE_ONE_BLOCK] & KEY_PRESS)
    //     deleteBlock();
	// if(keyState[KEY_DELETE_MORE_BLOCK] & KEY_HOLD)
    //     deleteBlock();
	if (keyState[KEY_DISPLAY_INFO] & KEY_PRESS)
	{ 
		std::cout << "chunk : " << (int)my_player.cameraPosition.x / Chunk::sizeX << " " << (int)my_player.cameraPosition.z / Chunk::sizeY
					<< " " << (int)my_player.cameraPosition.y << std::endl;
		std::cout << "pos : " << my_player.cameraPosition.x << " " << my_player.cameraPosition.y << " " << my_player.cameraPosition.z << std::endl;
	}

	my_player.yaw += mouseMoveX * sensitivity;
	my_player.pitch -= mouseMoveY * sensitivity;
	if(my_player.pitch > 89.0f)
		my_player.pitch =  89.0f;
	if(my_player.pitch < -89.0f)
		my_player.pitch = -89.0f;
	my_player.cameraDirection.x = cos(glm::radians(my_player.yaw)) * cos(glm::radians(my_player.pitch));
	my_player.cameraDirection.y = sin(glm::radians(my_player.pitch));
	my_player.cameraDirection.z = sin(glm::radians(my_player.yaw)) * cos(glm::radians(my_player.pitch));

	my_player.view = glm::lookAt(	my_player.cameraPosition, 
						my_player.cameraPosition + glm::normalize(my_player.cameraDirection),
						cameraUp);
}

int Game::GetRenderDistance() const {
	return renderDistance;
}

int	Game::GetChunkLoadingSize() const {
	return chunkLoadingSize;
}

glm::mat4	Game::GetCameraView() const {
	return my_player.view;
}
