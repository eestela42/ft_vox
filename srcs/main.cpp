#include <classes/Game/Game.hpp>
#include <classes/Profiler.hpp>
#include <thread>

int main(int argc, __attribute__((unused)) char **argv) {
	if (argc > 1) {
		Profiler::SetSaveOn();
	}
	unsigned int numThreads = std::thread::hardware_concurrency();
	std::cout << "numthread : " << numThreads <<std::endl;
	Profiler::StartTracking("Game Constructor");
	Game *game = new Game();
	Profiler::StopTracking("Game Constructor");
	
	game->StartLoop();
	Profiler::LogData();
	delete game;
	return 0;
}