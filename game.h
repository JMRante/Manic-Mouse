#pragma once
#ifndef MANICMOUSE_GAME_H_
#define MANICMOUSE_GAME_H_

#include <SDL.h>

#include "game_state.h"
#include "input_state.h"
#include "renderer.h"

class Game
{
public:
	bool Start();
	void Run();
	void Close();
private:
	SDL_Window* window;
	SDL_GLContext gl_context;

	InputState input_state;
	GameState game_state;
	Renderer renderer;

	Assets assets;

	void UpdateInputState(InputState& inputState);
	void UpdateGameState(GameState& gameState, const InputState& inputState);
};

#endif