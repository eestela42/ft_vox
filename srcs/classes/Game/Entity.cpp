#include <classes/Game/Entity.hpp>

std::vector<Entity*> *Entity::entities = new std::vector<Entity*>();
class mesh *Entity::mesh =  new class mesh("object3d/42.obj");;
u_int Entity::maxId = 1;

bool InsideBlock(glm::vec3 pos);

bool entityCollision(glm::vec3 position, u_int my_id)
{
	for (int i = 0; i < (*Entity::entities).size(); i++)
	{
		if ((*Entity::entities)[i]->GetId() != my_id)
		{
			glm::vec3 entity_position = (*Entity::entities)[i]->GetPosition();
			if ((glm::distance(position, entity_position) < 2.0f)
				&& position.x < entity_position.x + 0.5f && position.x > entity_position.x - 0.5f
				&& position.z < entity_position.z + 0.5f && position.z > entity_position.z - 0.5f)
				return true;
		}
	}
	return false;
}

Entity::Entity()
{
	id = maxId++;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	shader = NULL;
	vao = NULL;
	mesh = NULL;
}

Entity::Entity(glm::vec3 position, ShaderHandler *shaderHandler)
{
	id = maxId++;
	shader = shaderHandler->GetShader(shaderName);
	//ne pas dupliquer les vao
	std::cout << "creating entity" << std::endl;
	std::cout << "mesh shape assembly size : " << mesh->shapeAssemblyData.size() << std::endl;
	vao = new VertexArrayObject(new VertexBufferObject(mesh->dataStruct), new ElementBufferObject(mesh->shapeAssemblyData), shader);
	this->position = position;
}

Entity::Entity(glm::vec3 position,  Shader *shader)
{
	id = maxId++;
	shader = shader;
	//ne pas dupliquer les vao
	vao = new VertexArrayObject(new VertexBufferObject(mesh->dataStruct), new ElementBufferObject(mesh->shapeAssemblyData), shader);
	this->position = position;
}

Entity::~Entity()
{
}



void Entity::draw(glm::mat4 &matrix, glm::vec3 cameraPosition)
{
	vao->Bind();
	Shader::GetActiveShader()->Setmat4("modelView", matrix);
	Shader::GetActiveShader()->SetBool("isColor", false);
	Shader::GetActiveShader()->SetFloat("zoom", 1.0f);
	Shader::GetActiveShader()->SetFloat4("objectPos", position.x, position.z, position.y, 0);
	Shader::GetActiveShader()->SetFloat4("cameraPos", cameraPosition.x, cameraPosition.z, cameraPosition.y, 0);

	glDrawElements(GL_TRIANGLES, mesh->shapeAssemblyData.size() * 3, GL_UNSIGNED_INT, 0);
	vao->Unbind();
}

void Entity::Gravity()
{
	int chunkX = (int)floor(position.x / Chunk::sizeX);
	int chunkY = (int)floor(position.z / Chunk::sizeY);

	int blockX = (int)floor(position.x) - chunkX * Chunk::sizeX;
	int blockY = (int)floor(position.z) - chunkY * Chunk::sizeY;
	int blockZ = (int)floor(position.y);	
	for (int i = 0; i < Chunk::loadedChunks.size(); i++) {
	for (int j = 0; j < Chunk::loadedChunks[i].size(); j++) {

			if (Chunk::loadedChunks[i][j] && Chunk::loadedChunks[i][j]->GetX() == chunkX && Chunk::loadedChunks[i][j]->GetY() == chunkY)
			{
				if (veloDown > -1 && !Chunk::loadedChunks[i][j]->isFilled(blockX, blockY, blockZ - 2))
				{
					veloDown -= 0.1;
				}
				else if (veloDown < 0)
					veloDown = 0;

				if (veloDown > 0 && Chunk::loadedChunks[i][j]->isFilled(blockX, blockY, blockZ))
					veloDown = 0;
				position.y += veloDown;
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

void Entity::Jump()
{
	int chunkX = (int)floor(position.x / Chunk::sizeX);
	int chunkY = (int)floor(position.z / Chunk::sizeY);

	int blockX = (int)floor(position.x) - chunkX * Chunk::sizeX;
	int blockY = (int)floor(position.z) - chunkY * Chunk::sizeY;
	int blockZ = (int)floor(position.y);	
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



void Entity::Movement(glm::vec3 target)
{
	glm::vec3 movement = glm::normalize(target - position);
	glm::vec3 new_position = position + movement * 0.25f;
	if (InsideBlock(new_position) || InsideBlock(new_position + movement * 0.3f))
	{
		Jump();
		return ;
	}

	new_position.y -= 1;
	if (InsideBlock(new_position) || InsideBlock(new_position + movement * 0.3f))
	{
		Jump();
		return ;
	}

	if (entityCollision(new_position, this->id))
		return ;
	position.x = new_position.x;
	position.z = new_position.z;
}

glm::vec3 Entity::GetPosition() const
{
	return position;
}

u_int Entity::GetId() const
{
	return id;
}

glm::vec3 Entity::getHitbox() const
{
	return hitbox;
}


int Entity::getHealth() const
{
	return health;
}

int Entity::lowerHealth(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		for (int i = 0; i < (*Entity::entities).size(); i++)
		{
			if ((*Entity::entities)[i]->GetId() == id)
			{
				// (*Entity::entities).erase((*Entity::entities).begin() + i);
				// delete this;
				std::cout << "entity " << id << " died" << std::endl;
				health = 0;
				return 0;
			}
		}
	}
	return health;
}

void Entity::upperHealt(int heal)
{
	this->health += heal;
}

