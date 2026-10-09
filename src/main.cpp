#include "game.h"
#include "Logging/logging.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	logging::create(logging::Severity::DEBUG);
	Game game;
	game.run();
	return 0;
}