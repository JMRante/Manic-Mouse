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

struct Shooter {
	Transform transform;
	Sprite sprite;
};

struct Bullet {
	Transform transform;
	Sprite sprite;
};

// Player
// Cheese
// 3 Keys
// 3 Doors
// 12 shooters
// 120 bullet pool
// Tilemap
struct LevelState {
	Mouse mouse;
	Cheese cheese;
};

struct GameState {
	MetaMode mode;
	int level_id;
	LevelState level;
};

#endif