#include <classes/Game/Player.hpp>

bool InsideBlock(glm::vec3 pos) // place holder, i deleted the function
{
	return false;
}

Player::Player()
{
	fast = 1.5f;
	if (flyMode)
		fast = 20.0f;
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

void calculateRange(float pos, float dir, float entity_pos, float hitbox, glm::vec2 &range, float max_distance)
{
	bool inside = false;
	if (pos > entity_pos && pos < entity_pos + hitbox)
		bool inside = true;

	if (!dir)
	{
		if (!inside)
			return ;
		if (inside)
		{
			range.y = max_distance;
			return ;
		}
	}
	// box = {pos, pos + hitbox}
	float distance = 0;
	if (!inside)
	{
		distance = abs((entity_pos - pos) / dir);
		if (distance > max_distance)
			return ;
		range.x = distance;
	}
	
	float new_pos = pos + distance;
	distance = abs((entity_pos + hitbox - new_pos) / dir);
	if (new_pos + distance > max_distance)
	{
		range.y = max_distance;
		return ;
	}
	range.y = new_pos + distance;

}

bool overlap(glm::vec2 &range1, glm::vec2 &range2, glm::vec2 range3)
{
	float start = std::max(range1.x, std::max(range2.x, range3.x));
	float end = std::min(range1.y, std::min(range2.y, range3.y));
	if (start > end)
		return false;
	return true;	
}

bool collision(glm::vec3 position, glm::vec3 direction, float max_distance, glm::vec3 entity_position, glm::vec3 hitbox)
{
	std::cout << "position: " << position.x << " " << position.y << " " << position.z << std::endl;
	std::cout << "direction: " << direction.x << " " << direction.y << " " << direction.z << std::endl;
	std::cout << "entity_position: " << entity_position.x << " " << entity_position.y << " " << entity_position.z << std::endl;
	std::cout << "hitbox: " << hitbox.x << " " << hitbox.y << " " << hitbox.z << std::endl;
	if (position.x < entity_position.x && direction.x < 0 || position.x > entity_position.x + hitbox.x && direction.x > 0)
		return false;
	if (position.y < entity_position.y && direction.y < 0 || position.y > entity_position.y + hitbox.y && direction.y > 0)
		return false;
	if (position.z < entity_position.z && direction.z < 0 || position.z > entity_position.z + hitbox.z && direction.z > 0)
		return false;
	std::cout << "ok" << std::endl;
	
	glm::vec2 ranges[3] = {glm::vec2(0), glm::vec2(0), glm::vec2(0)};
	calculateRange(position.x, direction.x, entity_position.x, hitbox.x, ranges[0], max_distance);
	calculateRange(position.y, direction.y, entity_position.y, hitbox.y, ranges[1], max_distance);
	calculateRange(position.z, direction.z, entity_position.z, hitbox.z, ranges[2], max_distance);
	std::cout << "range 0: " << ranges[0].x << " " << ranges[0].y << std::endl;
	std::cout << "range 1: " << ranges[1].x << " " << ranges[1].y << std::endl;
	std::cout << "range 2: " << ranges[2].x << " " << ranges[2].y << std::endl;
	return overlap(ranges[0], ranges[1], ranges[2]);
}

void Player::playerAttack(std::vector<Entity*> &entities)
{
	glm::vec3 direction = cameraDirection;
	glm::vec3 position = cameraPosition;
	float max_distance_check = 6;
	float max_distance_hit = 5;

	for (Entity *entity : entities)
	{
		int entity_distance = glm::length(entity->GetPosition() - position);
		std::cout << "id: " << entity->GetId() << std::endl;
		if (entity_distance > max_distance_check)
		{
			std::cout << "too far" << std::endl;
			continue ;
		}
		glm::vec3 entity_position = entity->GetPosition();
		// entity_position.y -= entity->getHitbox().y;
		// entity_position.x -= entity->getHitbox().x;
		// entity_position.z -= entity->getHitbox().z;
		glm::vec3 hitbox = entity->getHitbox();
		// hitbox.y *= 2;
		// hitbox.x *= 2;
		// hitbox.z *= 2;
		if (collision(position, direction, max_distance_hit, entity_position, hitbox))
		{
			std::cout << "hit" << std::endl;
			entity->lowerHealth(500);
			return ;
		}
		std::cout << "miss" << std::endl;
	}
}

