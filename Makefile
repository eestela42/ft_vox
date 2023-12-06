NAME = ft_vox
CC = g++

CFLAGS =  -std=c++11
OPENGL = -lglfw3 -lGL -lX11


CPP_GAME = 		Game.cpp					\
				InputHandler.cpp

CPP_TEXTURE = 	Texture.cpp					\
				TextureLoader.cpp

CPP_VAO = 		VertexArrayObject.cpp		\
				VertexArrayObjectHandler.cpp 

CPP_WORLD = 	Chunk.cpp 					\
				ChunkRLE.cpp						\
				PerlinNoise.cpp


CLASSES = 	$(addprefix Game/, $(CPP_GAME))			\
			$(addprefix Texture/, $(CPP_TEXTURE))	\
			$(addprefix VAO/, $(CPP_VAO))			\
			$(addprefix World/, $(CPP_WORLD))		\
			ElementBufferObject.cpp 				\
			VertexBufferObject.cpp 					\
			Shader.cpp								\
			Window.cpp


SRCS =	main.cpp		\
		glad.c			\
		stb_image.cpp

SOURCES = 	$(addprefix srcs/classes/, $(CLASSES)) 		\
			$(addprefix srcs/, $(SRCS))

all: COMP

COMP: $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -Llibs -Iincludes $(OPENGL) -o $(NAME)

clean:
	rm -rf $(NAME)

fclean: clean
	rm -rf sources/classes/*.o
	rm -rf sources/*.o


re: fclean all

.PHONY: all clean fclean re