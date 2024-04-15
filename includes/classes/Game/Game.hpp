#ifndef TEMP_H
#define TEMP_H

#include <interfaces/Interface_Input.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <classes/World/ChunkDefault.hpp>
#include <classes/World/ChunkRLE.hpp>
#include <classes/Window.hpp>
#include <classes/Game/InputHandler.hpp>
#include <classes/ShaderHandler.hpp>
#include <classes/VAO/VertexArrayObjectHandler.hpp>
#include <classes/World/ChunkInstantiator.hpp>
#include <classes/Text.hpp>
#include <thread>

class Game : I_Input
{
	private:
		Window *window;
		InputHandler *inputHandler;
		ShaderHandler *shaderHandler;
		VertexArrayObjectHandler *vertexArrayObjectHandler;
		ChunkInstantiator *instantiator;

		void Loop();
	private:
		static glm::vec3 const cameraUp;
		int totalLoadWork;
		Text *loadingPercentageText;
		Text *fpsText;
		Text *frameTimeText;
		Text *chunkGenerationText;
		Text *chunkCompilationText;
		Text *chunkUpdateText;
		Text *playerPosXText;
		Text *playerPosYText;
		Text *playerPosZText;
		std::chrono::_V2::system_clock::time_point frameTimestamp;
		std::chrono::duration<float, std::nano> EMA;
		int renderDistance = 32;
		int targetFPS = 144;
		std::chrono::_V2::system_clock::time_point beforeSleep;
		std::chrono::nanoseconds correction = std::chrono::nanoseconds(0);
		int chunkLoadingSize = renderDistance * 2 + 1;
		const float speed = 0.05f;
		const float sensitivity = 0.05f;

		float yaw = -90;
		float pitch = 0;
		glm::vec3 cameraPosition = glm::vec3(0, 30, 0);
		glm::vec3 cameraDirection = glm::vec3(	cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
												sin(glm::radians(pitch)),
												sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
		glm::mat4 view = glm::lookAt(	cameraPosition,
										cameraPosition + cameraDirection,
										glm::vec3(0, 1, 0));
	public:
		Game();

		void SendKeys(u_char *keyState, double mouseMoveX, double mouseMoveY) override;
		void StartLoop();

		glm::mat4 GetCameraView() const;
		int	GetRenderDistance() const;
		int	GetChunkLoadingSize() const;

		~Game();
};

#endif