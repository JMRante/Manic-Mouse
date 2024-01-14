#pragma once
#ifndef MANICMOUSE_GAME_MATH_H_
#define MANICMOUSE_GAME_MATH_H_

struct Vector2
{
	float x;
	float y;
};

struct Matrix4
{
	float data[16];

	Matrix4();
	Matrix4(Vector2 translate, float rotate_radians_on_z, Vector2 scale);

	static Matrix4 Multiply(const Matrix4& matrix_left, const Matrix4& matrix_right);
};

#endif