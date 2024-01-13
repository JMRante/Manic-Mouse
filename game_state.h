#pragma once
#ifndef MANICMOUSE_GAME_STATE_H_
#define MANICMOUSE_GAME_STATE_H_

enum MetaMode {
	Main_Menu,
	Gameplay
};

class Transform {
public:
	Transform();

	float GetX();
	float GetY();
	float GetScaleX();
	float GetScaleY();
	float GetRotationInRadians();
	float* GetTransformMatrix();

	void SetX(float x);
	void SetY(float y);
	void SetScaleX(float scale_x);
	void SetScaleY(float scale_y);
	void SetRotationInRadians(float rotation);
private:
	float x;
	float y;
	float scale_x;
	float scale_y;
	float rotation_radians;
	float transform_matrix[16];
};

struct Sprite {
	int width;
	int height;
	int offset_x;
	int offset_y;
};

struct Mouse {
	Transform transform;
	Sprite sprite;
};

struct GameState {
	MetaMode mode;
	int level_id;
	Mouse mouse;
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