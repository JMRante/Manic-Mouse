#pragma once
#ifndef MANICMOUSE_GAME_STATE_H_
#define MANICMOUSE_GAME_STATE_H_

#include "game_math.h"

enum MetaMode {
	Main_Menu,
	Gameplay
};

enum KeyColor {
	Red,
	Yellow,
	Blue
};

enum MovingBlockBehavior {
	Horizontal,
	Vertical
};

struct Transform {
	Transform();

	Vector2D GetPosition();
	Vector2D GetScale();
	float GetRotationInRadians();
	Matrix4D GetTransformMatrix();

	void SetPosition(Vector2D position);
	void SetPositionX(float x);
	void SetPositionY(float y);
	void SetScale(Vector2D scale);
	void SetRotationInRadians(float rotation_radians);
private:
	Vector2D position;
	Vector2D scale;
	float rotation_radians;
	Matrix4D transform_matrix;
};

struct Sprite {
	Vector2D size;
	Vector2D offset;
};

struct Mouse {
	bool active;
	Transform transform;
	Sprite sprites[4];
	int sprite_index;
	float sprite_timer;
	float sprite_speed;
};

struct Cheese {
	bool active;
	Transform transform;
	Sprite sprite;
};

struct Key {
	bool active;
	KeyColor color;
	Transform transform;
	Sprite sprite;
};

struct Door {
	bool active;
	KeyColor color;
	Transform transform;
	Sprite sprite;
};

struct MovingBlock {
	bool active;
	MovingBlockBehavior behavior;
	Transform transform;
	Sprite sprite;
};

struct Tilemap {
	bool active;
	int tilesheet_index;
	unsigned char tiles[920]; // 40 x 23

	unsigned char GetTileByCoordinate(const int x, const int y);
	Vector2D TilemapIndexToTilesheetOffset(const int tilemap_index);
	bool IsPointInWall(const Vector2D point);
};

// Player
// Cheese
// 3 Keys
// 3 Doors
// 20 Moving Blocks
// Tilemap
struct LevelState {
	LevelState();

	Vector2D start;
	Mouse mouse;
	Cheese cheese;
	Key red_key;
	Key yellow_key;
	Key blue_key;
	Door red_door;
	Door yellow_door;
	Door blue_door;
	int moving_block_count;
	MovingBlock moving_blocks[20];
	Tilemap tilemap;
};

struct GameState {
	MetaMode mode;
	int level_id;
	LevelState level;
};

#endif