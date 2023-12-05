NAME = vox
CC = g++

CFLAGS =  -std=c++11
OPENGL = -lglfw3 -lGL -lX11

SRCS =	main.cpp	\
		glad.c		\
		stb_image.cpp
		
CLASS = Shader.cpp						\
		TextureLoader.cpp 				\
		Texture.cpp						\
		Window.cpp						\
		InputHandler.cpp 				\
		VertexArrayObject.cpp 			\
		Game.cpp						\
		VertexArrayObjectHandler.cpp 	\
		ElementArrayBuffer.cpp 			

SOURCES = $(addprefix srcs/, $(SRCS)) \
			$(addprefix srcs/classes/, $(CLASS))

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