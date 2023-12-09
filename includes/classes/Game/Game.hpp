#ifndef TEMP_H
#define TEMP_H

#include <interfaces/Interface_Input.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Game : I_Input
{
	public:
		Game();
		~Game();

		void 		SendKeys(u_char *keyState, double mouseMoveX, double mouseMoveY) override;
		glm::mat4	GetCameraView() const;

		int			GetRenderDistance() const;
	private:
		static glm::vec3 const cameraUp;
		float yaw = -90;
		float pitch = 0;
		glm::vec3 cameraPosition = glm::vec3(0, 0, 0);
		glm::vec3 cameraDirection = glm::vec3(	cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
												sin(glm::radians(pitch)),
												sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
		glm::mat4 view = glm::lookAt(	cameraPosition,
										cameraPosition + cameraDirection,
										glm::vec3(0, 1, 0));
		
		int renderDistance = 2;
		const float speed = 0.10f;
		const float sensitivity = 0.05f;
};

#endif