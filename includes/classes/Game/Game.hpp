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
#include <classes/Texture/Texture.hpp>
#include <classes/Texture/TextureArray.hpp>
#include <classes/Game/SkyBox.hpp>
#include <classes/Game/CrossHair.hpp>
#include <classes/Game/Player.hpp>
#include <classes/Game/Entity.hpp>




class Game : I_Input
{
	private:
		Window *window;
		InputHandler *inputHandler;
		ShaderHandler *shaderHandler;
		VertexArrayObjectHandler *vertexArrayObjectHandler;
		ChunkInstantiator *instantiator;
		Player my_player;

		void Loop();
	private:
		static glm::vec3 const cameraUp;
		int renderDistance = 1;
		int chunkLoadingSize = renderDistance * 2 + 1;
		const float speed = 0.35f;
		const float sensitivity = 0.05f;
		bool isEnd = false;

		
	public:
		Game();

		int tmp = 0;
		
		SkyBox *skyBox;
		CrossHair *crossHair;
		void SendKeys(u_char *keyState, double mouseMoveX, double mouseMoveY) override;
		void StartLoop();
		void generationLoop();

		glm::mat4 GetCameraView() const;
		int	GetRenderDistance() const;
		int	GetChunkLoadingSize() const;

		Texture blockTexture;
		TextureArray blockTextureArray;


		bool	putBlock(glm::vec3 pos, u_char type);
		void 	deleteBlock();
		glm::vec4 findColorFilter();
		virtual ~Game();
};

// void addVaoCreateQueue(t_vertexData *vertexData, std::vector<unsigned int> *indices);
// void addVaoDeleteQueue(u_int vao);

// void vaoCreate(VertexArrayObjectHandler *vaoHandler, ShaderHandler *shaderHandler);
// void vaoDelete(VertexArrayObjectHandler *vaoHandler);


#endif