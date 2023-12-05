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

		void 		SendKeys(u_char *keyState) override;
		glm::quat 	GetCameraQuaternion() const;
		glm::vec3 	GetCameraPosition() const;
	private:
		glm::quat cameraQuaternion = glm::angleAxis(float(0), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, -10.0f);
		const float speed = 0.05f;
};

#endif