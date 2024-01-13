#pragma once
#ifndef MANICMOUSE_GAME_STATE_H_
#define MANICMOUSE_GAME_STATE_H_

enum MetaMode {
	Main_Menu,
	Gameplay
};

struct Sprite {
	int width;
	int height;
	int offset_x;
	int offset_y;
};

struct GameState {
	MetaMode mode;
	int level_id;
	Sprite test_sprite;
};

// Player
// Cheese
// 3 Keys
// 3 Doors
// 12 shooters
// 120 bullet pool
// Tilemap
struct LevelState {

};

#endif