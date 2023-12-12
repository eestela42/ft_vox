#include <classes/Game/Game.hpp>
#include <classes/Profiler.hpp>

int main(int argc, char **argv) {
	if (argc > 1) {
		Profiler::SetSaveOn();
	}
	Profiler::StartTracking("Game Constructor");
	Game *game = new Game();
	Profiler::StopTracking("Game Constructor");
	
	game->StartLoop();
	Profiler::LogData();
	return 0;
}