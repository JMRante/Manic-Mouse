#pragma once
#ifndef MANICMOUSE_GAME_MATH_H_
#define MANICMOUSE_GAME_MATH_H_

struct Vector2
{
	float x;
	float y;

	friend Vector2 operator+(const Vector2& vector_left, const Vector2& vector_right);
	friend Vector2 operator-(const Vector2& vector_left, const Vector2& vector_right);
	friend Vector2 operator*(const Vector2& vector_left, const Vector2& vector_right);
	friend Vector2 operator*(const Vector2& vector, float scalar);
	friend Vector2 operator*(float scalar, const Vector2& vector);
	Vector2& operator*=(float scalar);
	Vector2& operator+=(const Vector2& vector_right);
	Vector2& operator-=(const Vector2& vector_right);

	float Length();
};

struct Matrix4
{
	float data[16];

	Matrix4();
	Matrix4(Vector2 translate, float rotate_radians_on_z, Vector2 scale);

	static Matrix4 Multiply(const Matrix4& matrix_left, const Matrix4& matrix_right);
};

#endif