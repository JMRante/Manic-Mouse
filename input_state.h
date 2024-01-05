#pragma once
#ifndef MANICMOUSE_INPUT_STATE_H_
#define MANICMOUSE_INPUT_STATE_H_

struct InputState
{
	int mouse_x;
	int mouse_y;
	bool mouse_left_pressed = false;
	bool window_closed = false;
};

#endif