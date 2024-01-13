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

	Vector2 GetPosition();
	Vector2 GetScale();
	float GetRotationInRadians();
	Matrix4 GetTransformMatrix();

	void SetPosition(Vector2 position);
	void SetScale(Vector2 scale);
	void SetRotationInRadians(float rotation);
private:
	Vector2 position;
	Vector2 scale;
	float rotation_radians;
	Matrix4 transform_matrix;
};

struct Sprite {
	Vector2 size;
	Vector2 offset;
};

struct Mouse {
	Vector2 direction_history[10];
	Transform transform;
	Sprite sprite;
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