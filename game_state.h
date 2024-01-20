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

struct Transform {
	Transform();

	Vector2D GetPosition();
	Vector2D GetScale();
	float GetRotationInRadians();
	Matrix4D GetTransformMatrix();

	void SetPosition(Vector2D position);
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
	Transform transform;
	Sprite sprites[4];
	int sprite_index;
	float sprite_timer;
	float sprite_speed;
};

struct Cheese {
	Transform transform;
	Sprite sprite;
};

struct Key {
	KeyColor color;
	Transform transform;
	Sprite sprite;
};

struct Door {
	KeyColor color;
	Transform transform;
	Sprite sprite;
};

struct Tilemap {
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
// Tilemap
struct LevelState {
	Mouse mouse;
	Cheese cheese;
	Tilemap tilemap;
};

struct GameState {
	MetaMode mode;
	int level_id;
	LevelState level;
};



#endif