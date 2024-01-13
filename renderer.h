#pragma once
#ifndef MANICMOUSE_RENDERER_H_
#define MANICMOUSE_RENDERER_H_

#include <SDL.h>

#include "assets.h"
#include "game_state.h"

class Renderer
{
public:
	SDL_Window* window;
	SDL_GLContext gl_context;

	int window_width;
	int window_height;

	bool Load();
	void Unload();

	void Render(const GameState& game_state, const Assets& assets);
};

#endif