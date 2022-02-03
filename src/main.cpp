#include "GameController.hpp"

int main()
{
	match3::GameController game = match3::GameController("Resources/");
	game.run();

	return EXIT_SUCCESS;
}