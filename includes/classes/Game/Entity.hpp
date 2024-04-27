#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <interfaces/Interface_Input.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <classes/World/Chunk.hpp>
#include <classes/Game/mesh.hpp>
#include <classes/VAO/VertexArrayObject.hpp>
#include <classes/ShaderHandler.hpp>



class mesh;

class Entity
{
	private :

	glm::vec3 position;
	glm::vec3 hitbox = {2.0f , 2.0f, 2.0f};
	float veloDown = 0;
	u_int id;
	int health = 100;
	
	public :

	static constexpr char * shaderName = (char*)"object";
	Shader * shader;

	static class mesh *mesh;
	static u_int maxId;
	static std::vector<Entity*> *entities;
	VertexArrayObject *vao;

	~Entity();
	Entity();
	Entity(glm::vec3 position, ShaderHandler *shaderHandler);
	Entity(glm::vec3 position,  Shader *shader);


	void draw(glm::mat4 &matrix, glm::vec3 position);
	void Gravity();
	void Movement(glm::vec3 target);
	void Jump();

	glm::vec3 GetPosition() const;
	u_int GetId() const;
	glm::vec3 getHitbox() const;
	int getHealth() const;
	int lowerHealth(int damage);
	void upperHealt(int heal);

};

bool entityCollision(glm::vec3 position, u_int my_id);

#endif


