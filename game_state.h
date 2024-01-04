#pragma once
#ifndef MANICMOUSE_GAME_STATE_H_
#define MANICMOUSE_GAME_STATE_H_

enum MetaMode {
	Main_Menu,
	Gameplay
};

// Game State (memory allocated during initialization then released on quit)
struct GameState {
	MetaMode mode;
	int level_id;
};

// Level State (memory allocated during level load then released on level close)
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