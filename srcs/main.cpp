#include <classes/Game/Game.hpp>

int main(int argc, char **argv) {
	Game *game = new Game();

	game->StartLoop();
	return 0;
}