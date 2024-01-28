#pragma once
#ifndef MANICMOUSE_INPUT_STATE_H_
#define MANICMOUSE_INPUT_STATE_H_

#include "game_math.h"

struct InputState {
	Vector2D mouse_direction_history[10];
	Vector2D mouse_direction_smoothed;
	Vector2D mouse_position;
	Vector2D last_mouse_position;
	float mouse_speed;
	bool window_closed = false;
};

#endif