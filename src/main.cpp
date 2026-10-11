#include "game.h"
#include "Logging/logging.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	logging::create(logging::Severity::DEBUG); //TODO : mettre dans game.init() ?
	Game game; //TODO : renommer en Engine ?
	game.run();
	return 0;
}