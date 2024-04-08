#include <classes/Game/Player.hpp>



Player::Player()
{
}

Player::~Player()
{
}

void Player::Gravity()
{
	if (flyMode == true)
		return ;
	int chunkX = (int)floor(cameraPosition.x / Chunk::sizeX);
	int chunkY = (int)floor(cameraPosition.z / Chunk::sizeY);

	int blockX = (int)floor(cameraPosition.x) - chunkX * Chunk::sizeX;
	int blockY = (int)floor(cameraPosition.z) - chunkY * Chunk::sizeY;
	int blockZ = (int)floor(cameraPosition.y);	
	for (int i = 0; i < Chunk::loadedChunks.size(); i++) {
	for (int j = 0; j < Chunk::loadedChunks[i].size(); j++) {

			if (Chunk::loadedChunks[i][j] && Chunk::loadedChunks[i][j]->GetX() == chunkX && Chunk::loadedChunks[i][j]->GetY() == chunkY)
			{
				if (veloDown > -1 && !Chunk::loadedChunks[i][j]->isFilled(blockX, blockY, blockZ - 2))
				{
					veloDown -= 0.05;
				}
				else if (veloDown < 0)
					veloDown = 0;

				if (veloDown > 0 && Chunk::loadedChunks[i][j]->isFilled(blockX, blockY, blockZ))
					veloDown = 0;
				cameraPosition.y += veloDown;
				// else if (
				// 			Chunk::loadedChunks[i][j]->isFilled(blockX, blockY, blockZ)
				// 			|| Chunk::loadedChunks[i][j]->isFilled(blockX, blockY, blockZ - 1))
				// 	cameraPosition.y += 0.5;
				return ;
			}
		{
		}
	}
	}
}

void Player::Jump()
{
	if (flyMode == true)
	{
		cameraPosition += speedMultiplier * speed * cameraUp;
		return ;
	}
	int chunkX = (int)floor(cameraPosition.x / Chunk::sizeX);
	int chunkY = (int)floor(cameraPosition.z / Chunk::sizeY);

	int blockX = (int)floor(cameraPosition.x) - chunkX * Chunk::sizeX;
	int blockY = (int)floor(cameraPosition.z) - chunkY * Chunk::sizeY;
	int blockZ = (int)floor(cameraPosition.y);	
	for (int i = 0; i < Chunk::loadedChunks.size(); i++) {
	for (int j = 0; j < Chunk::loadedChunks[i].size(); j++) {

			if (Chunk::loadedChunks[i][j] && Chunk::loadedChunks[i][j]->GetX() == chunkX && Chunk::loadedChunks[i][j]->GetY() == chunkY)
			{
				if (Chunk::loadedChunks[i][j]->isFilled(blockX, blockY, blockZ)
				|| Chunk::loadedChunks[i][j]->isFilled(blockX, blockY, blockZ - 2)
				|| Chunk::loadedChunks[i][j]->isFilled(blockX, blockY, blockZ - 1))
					veloDown = 0.3;
				return ;
			}
		{
		}
	}
	}
}

void Player::Sneak()
{
	if (flyMode == true)
	{
		cameraPosition -= speedMultiplier * speed * cameraUp;
		return ;
	}
	

}



void Player::PlayerMove(u_char direction)
{
	glm::vec3 movement;
	switch (direction)
	{
	case KEY_MOVE_FORWARD:
		movement = speedMultiplier * speed * cameraDirection;
		break;
	case KEY_MOVE_BACKWARD:
		movement = -speedMultiplier * speed * cameraDirection;
		break;
	case KEY_MOVE_RIGHTWARD:
		movement = speedMultiplier * speed * glm::normalize(glm::cross(cameraDirection, cameraUp));
		break;
	case KEY_MOVE_LEFTWARD:
		movement = -speedMultiplier * speed * glm::normalize(glm::cross(cameraDirection, cameraUp));
		break;
	default:
		return;
	}
	if (flyMode)
	{
		cameraPosition += movement;
		return ;
	}
	glm::vec3 new_position = cameraPosition + movement;
	// ChunkRLE tmp(0, 0); //ceci etre moche
	if (InsideBlock(new_position) || InsideBlock(new_position + movement * 0.3f))
		return;
	new_position.y -= 1;
	if (InsideBlock(new_position) || InsideBlock(new_position + movement * 0.3f))
		return;

	cameraPosition.x = new_position.x;
	cameraPosition.z = new_position.z;

}