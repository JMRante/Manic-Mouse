#pragma once
#ifndef MANICMOUSE_GAME_MATH_H_
#define MANICMOUSE_GAME_MATH_H_

struct Vector2D
{
	float x;
	float y;

	friend Vector2D operator+(const Vector2D& vector_left, const Vector2D& vector_right);
	friend Vector2D operator-(const Vector2D& vector_left, const Vector2D& vector_right);
	friend Vector2D operator*(const Vector2D& vector_left, const Vector2D& vector_right);
	friend Vector2D operator*(const Vector2D& vector, float scalar);
	friend Vector2D operator*(float scalar, const Vector2D& vector);
	Vector2D& operator*=(float scalar);
	Vector2D& operator+=(const Vector2D& vector_right);
	Vector2D& operator-=(const Vector2D& vector_right);

	float Length() const;
	static float Dot(const Vector2D& vector_left, const Vector2D& vector_right);
	static float Angle(const Vector2D& vector_left, const Vector2D& vector_right);
	static Vector2D Normalized(const Vector2D& vector);

	static const Vector2D zero;
	static const Vector2D right;
	static const Vector2D up;
};

struct Matrix4D
{
	float data[16];

	Matrix4D();
	Matrix4D(Vector2D translate, float rotate_radians_on_z, Vector2D scale);

	friend Matrix4D operator*(const Matrix4D& matrix_left, const Matrix4D& matrix_right);
};

#endif