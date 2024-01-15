#include <SDL.h>
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>

#include "game.h"

int main(int argc, char** args) {
	Game game;
	
	if (game.Start()) {
		game.Run();
	}

	game.Close();

	return 0;
}