#pragma once
#ifndef MANICMOUSE_GAME_MATH_H_
#define MANICMOUSE_GAME_MATH_H_

struct Vector2D {
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
	float LengthSquared() const;
	static float Dot(const Vector2D& vector_left, const Vector2D& vector_right);
	static float Angle(const Vector2D& vector_left, const Vector2D& vector_right);
	static Vector2D Normalized(const Vector2D& vector);

	static const Vector2D zero;
	static const Vector2D right;
	static const Vector2D up;
};

struct Matrix4D {
	float data[16];

	Matrix4D();
	Matrix4D(Vector2D translate, float rotate_radians_on_z, Vector2D scale);

	friend Matrix4D operator*(const Matrix4D& matrix_left, const Matrix4D& matrix_right);
};

float Lerp(float start, float end, float t);
Vector2D LerpVector2D(const Vector2D& start, const Vector2D& end, float t);

bool IsPointCollidingWithCircle(const Vector2D& point, const Vector2D& circle_origin, float circle_radius);
bool IsContinuousPointCollidingWithTileArray(const Vector2D& previous_point, const Vector2D& current_point, Vector2D& collision_point, unsigned char* tile_data);

#endif