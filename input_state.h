#pragma once
#ifndef MANICMOUSE_INPUT_STATE_H_
#define MANICMOUSE_INPUT_STATE_H_

#include "game_math.h"

struct InputState
{
	Vector2 mouse_position;
	bool mouse_left_pressed = false;
	bool window_closed = false;
};

#endif