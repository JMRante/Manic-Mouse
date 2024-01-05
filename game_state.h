#pragma once
#ifndef MANICMOUSE_GAME_STATE_H_
#define MANICMOUSE_GAME_STATE_H_

enum MetaMode {
	Main_Menu,
	Gameplay
};

struct GameState {
	MetaMode mode;
	int level_id;
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