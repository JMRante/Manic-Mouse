#pragma once
#ifndef MANICMOUSE_INPUT_STATE_H_
#define MANICMOUSE_INPUT_STATE_H_

// Input State (memory allocated during initialization then released on quit)
struct InputState
{
	int mouse_x;
	int mouse_y;
	bool mouse_left_pressed;
	bool window_closed;
};

#endif