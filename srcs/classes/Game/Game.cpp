#include <main.hpp>

glm::vec3 const Game::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Game::Game() {
	window = new Window("ft_vox", DrawMode::FILL);

	inputHandler = new InputHandler(window->GetWindow());
	inputHandler->AddCallback((I_Input*)this);
	inputHandler->AddCallback((I_Input*)window);

	shaderHandler = new ShaderHandler("shaders");
	vertexArrayObjectHandler = new VertexArrayObjectHandler();

	Shader *shader = shaderHandler->GetShader("text");
	BitmapFont *font = new BitmapFont(shader, "font.png", 6, 16, 32);
	loadingPercentageText = new Text(window, font, 0xffffff00, 48, "Try this you bitch", 0,0);
	fpsText = new Text(window, font, 0xffffff00, 24, "", 0.8,1);
	frameTimeText = new Text(window, font, 0xffffff00, 24, "", 0.8,0.95);
	chunkGenerationText = new Text(window, font, 0xffffff00, 24, "", 0.8,0.9);
	chunkCompilationText = new Text(window, font, 0xffffff00, 24, "", 0.8,0.85);
	chunkUpdateText = new Text(window, font, 0xffffff00, 24, "", 0.8,0.80);
	playerPosXText = new Text(window, font, 0xffffff00, 24, "X", 0.8,0.75);
	playerPosYText = new Text(window, font, 0xffffff00, 24, "Y", 0.8,0.70);
	playerPosZText = new Text(window, font, 0xffffff00, 24, "Z", 0.8,0.65);

	vertexArrayObjectHandler->AddTextVAO(loadingPercentageText, new VertexArrayObject(new VertexBufferObject(loadingPercentageText->GetVertexData()), 0, shader));
	vertexArrayObjectHandler->AddTextVAO(fpsText, new VertexArrayObject(new VertexBufferObject(fpsText->GetVertexData()), 0, shader));
	vertexArrayObjectHandler->AddTextVAO(frameTimeText, new VertexArrayObject(new VertexBufferObject(frameTimeText->GetVertexData()), 0, shader));
	vertexArrayObjectHandler->AddTextVAO(chunkGenerationText, new VertexArrayObject(new VertexBufferObject(chunkGenerationText->GetVertexData()), 0, shader));
	vertexArrayObjectHandler->AddTextVAO(chunkCompilationText, new VertexArrayObject(new VertexBufferObject(chunkCompilationText->GetVertexData()), 0, shader));
	vertexArrayObjectHandler->AddTextVAO(chunkUpdateText, new VertexArrayObject(new VertexBufferObject(chunkUpdateText->GetVertexData()), 0, shader));
	vertexArrayObjectHandler->AddTextVAO(playerPosXText, new VertexArrayObject(new VertexBufferObject(playerPosXText->GetVertexData()), 0, shader));
	vertexArrayObjectHandler->AddTextVAO(playerPosYText, new VertexArrayObject(new VertexBufferObject(playerPosYText->GetVertexData()), 0, shader));
	vertexArrayObjectHandler->AddTextVAO(playerPosZText, new VertexArrayObject(new VertexBufferObject(playerPosZText->GetVertexData()), 0, shader));

	Chunk::setRenderDistance(renderDistance);
	totalLoadWork = (float)(renderDistance * renderDistance) * M_PI * 2;
	instantiator = new ChunkInstantiator(vertexArrayObjectHandler, renderDistance, shaderHandler);
}

void Game::StartLoop() {
	frameTimestamp = std::chrono::high_resolution_clock::now();
	EMA = std::chrono::milliseconds(10);
	beforeSleep = frameTimestamp;
	while(window->ShouldContinue())
	{
		Loop();
	}
}

void Game::Loop() {
	std::chrono::_V2::system_clock::time_point newFrameTimestamp = std::chrono::high_resolution_clock::now();
	float alpha = (float)((newFrameTimestamp - frameTimestamp).count()) / 50000000; //50ms
	alpha = (alpha > 1) ? 1 : alpha;
	EMA = alpha * (newFrameTimestamp - frameTimestamp) + (1.0f - alpha) * EMA;
	frameTimestamp = newFrameTimestamp;
	
	inputHandler->HandleInput();
	window->Clear();
	instantiator->Update(cameraPosition, std::chrono::milliseconds(10));

	if (totalLoadWork) {
		int workLeft = instantiator->GetWorkLeft();
		if (workLeft) {
			int progress = instantiator->GetWorkLeft() * 100 / totalLoadWork;
			if (progress > 100) {
				progress = 100;
			}
			progress = 100 - progress;
			loadingPercentageText->ChangeText(std::to_string(progress) + "%");
			vertexArrayObjectHandler->ReplaceTextVAO(loadingPercentageText, new VertexArrayObject(new VertexBufferObject(loadingPercentageText->GetVertexData()), 0, shaderHandler->GetShader("text")));
			vertexArrayObjectHandler->Draw(loadingPercentageText);
		}
		else {
			window->SetClearColor(55, 77, 144);
			totalLoadWork = 0;
			vertexArrayObjectHandler->RemoveTextVAO(loadingPercentageText);
		}
	}
	if (!totalLoadWork) {
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(2) << EMA.count() / 1000000;
		fpsText->ChangeText(std::to_string((int)(1000000000 / EMA.count())) + "fps");
		frameTimeText->ChangeText(oss.str() + "ms");
		chunkGenerationText->ChangeText("Generation : " + std::to_string(instantiator->GetGenerationsLeft()));
		chunkCompilationText->ChangeText("Compilation : " + std::to_string(instantiator->GetCompilationsLeft()));
		chunkUpdateText->ChangeText("Update : " + std::to_string(instantiator->GetUpdateLeft()));
		playerPosXText->ChangeText("X : " + std::to_string((int)cameraPosition.x));
		playerPosYText->ChangeText("Y : " + std::to_string((int)cameraPosition.y));
		playerPosZText->ChangeText("Z : " + std::to_string((int)cameraPosition.z));
		
		glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)window->GetWidth()/(float)window->GetHeight(), 0.1f, (float)renderDistance * 16 * 1.5f);
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = proj * GetCameraView();
		shaderHandler->Use("default");
		Shader::GetActiveShader()->Setmat4("matrix", matrix);
		vertexArrayObjectHandler->DrawAll();
	}
	
	window->SwapBuffersAndPollEvents();
	if (1000000000/targetFPS - (std::chrono::high_resolution_clock::now() - frameTimestamp).count() > 0) {
		std::chrono::nanoseconds ask = std::chrono::nanoseconds(1000000000/targetFPS) - (std::chrono::high_resolution_clock::now() - frameTimestamp) + correction;
		std::chrono::_V2::system_clock::time_point beforeSleep = std::chrono::high_resolution_clock::now();
		std::this_thread::sleep_for(ask);
		std::chrono::nanoseconds reality = std::chrono::high_resolution_clock::now() - beforeSleep;
		correction = std::chrono::nanoseconds(ask - reality);
	}
	else {
		correction = std::chrono::nanoseconds(0);
	}
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