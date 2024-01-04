#pragma once
#ifndef MANICMOUSE_RENDERER_H_
#define MANICMOUSE_RENDERER_H_

#include "assets.h"
#include "game_state.h"

class Renderer
{
public:
	void Render(const GameState& game_state, const Assets& assets);
};

#endif