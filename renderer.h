#pragma once
#ifndef MANICMOUSE_RENDERER_H_
#define MANICMOUSE_RENDERER_H_

#include <SDL.h>

#include "assets.h"
#include "game_state.h"

class Renderer {
public:
	SDL_Window* window;
	SDL_GLContext gl_context;

	int window_width;
	int window_height;

	Matrix4D view_matrix;

	Matrix4D tilemap_transform_matrix;
	Matrix4D transition_transform_matrix;

	bool Load(bool fullscreen);
	void Unload();

	void Render(GameState& game_state, Assets& assets);
private:
	void RenderSprite(Sprite& sprite, Transform& transform, Assets& assets);
};

#endif