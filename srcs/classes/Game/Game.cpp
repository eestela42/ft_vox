#include <classes/Game/Game.hpp>

Game::Game() {}

void Game::SendKeys(u_char *keyState) {
	if(keyState[KEY_MOVE_FORWARD] & KEY_HOLD)
        cameraPosition += glm::vec3(0, 0, -speed);
    if(keyState[KEY_MOVE_BACKWARD] & KEY_HOLD)
        cameraPosition += glm::vec3(0, 0, speed);
    if(keyState[KEY_MOVE_RIGHTWARD] & KEY_HOLD)
        cameraPosition += glm::vec3(speed, 0, 0);
    if(keyState[KEY_MOVE_LEFTWARD] & KEY_HOLD)
        cameraPosition += glm::vec3(-speed, 0, 0);
    if(keyState[KEY_MOVE_UPWARD] & KEY_HOLD)
        cameraPosition += glm::vec3(0, speed, 0);
    if(keyState[KEY_MOVE_DOWNWARD] & KEY_HOLD)
        cameraPosition += glm::vec3(0, -speed, 0);
}


glm::quat Game::GetCameraQuaternion() const {
    return cameraQuaternion;
}

glm::vec3 Game::GetCameraPosition() const {
    return cameraPosition;
}

Game::~Game() {}