# ifndef PLAYER_HPP
# define PLAYER_HPP


#include <iostream>
#include <interfaces/Interface_Input.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <classes/World/Chunk.hpp>
#include <classes/World/ChunkRLE.hpp>


class Player
{
	private :

	public :
		float yaw = -90;
		float pitch = 0;
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraPosition = glm::vec3(0, 255, 0);
		glm::vec3 cameraDirection = glm::vec3(	cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
												sin(glm::radians(pitch)),
												sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
		glm::mat4 view = glm::lookAt(	cameraPosition,
										cameraPosition + cameraDirection,
										glm::vec3(0, 1, 0));
		const float speed = 0.35f;
		float speedMultiplier = 1;
		const float sensitivity = 0.05f;
		float veloDown = 0;
		bool flyMode = true;

		Player();
		~Player();
		void Gravity();
		void Jump();
		void Sneak();
		void PlayerMove(u_char direction);

};

#endif