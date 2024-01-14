#include "game_math.h"

#include <cmath>

Vector2 operator+(const Vector2& vector_left, const Vector2& vector_right)
{
	return Vector2(vector_left.x + vector_right.x, vector_left.y + vector_right.y);
}

Vector2 operator-(const Vector2& vector_left, const Vector2& vector_right)
{
	return Vector2(vector_left.x - vector_right.x, vector_left.y - vector_right.y);
}

Vector2 operator*(const Vector2& vector_left, const Vector2& vector_right)
{
	return Vector2(vector_left.x * vector_right.x, vector_left.y * vector_right.y);
}

Vector2 operator*(const Vector2& vector, float scalar)
{
	return Vector2(vector.x * scalar, vector.y * scalar);
}

Vector2 operator*(float scalar, const Vector2& vector)
{
	return Vector2(vector.x * scalar, vector.y * scalar);
}

Vector2& Vector2::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

Vector2& Vector2::operator+=(const Vector2& vector_right)
{
	x += vector_right.x;
	y += vector_right.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& vector_right)
{
	x -= vector_right.x;
	y -= vector_right.y;
	return *this;
}

float Vector2::Length()
{
	return sqrt(x * x + y * y);
}

Matrix4::Matrix4() {
	data[0] = 1.0f;
	data[1] = 0.0f;
	data[2] = 0.0f;
	data[3] = 0.0f;

	data[4] = 0.0f;
	data[5] = 1.0f;
	data[6] = 0.0f;
	data[7] = 0.0f;

	data[8] = 0.0f;
	data[9] = 0.0f;
	data[10] = 1.0f;
	data[11] = 0.0f;

	data[12] = 0.0f;
	data[13] = 0.0f;
	data[14] = 0.0f;
	data[15] = 1.0f;
}

Matrix4::Matrix4(Vector2 translate, float rotate_radians_on_z, Vector2 scale) {
	data[0] = cos(rotate_radians_on_z) * scale.x;
	data[1] = -sin(rotate_radians_on_z);
	data[2] = 0.0f;
	data[3] = 0.0f;

	data[4] = sin(rotate_radians_on_z);
	data[5] = cos(rotate_radians_on_z) * scale.y;
	data[6] = 0.0f;
	data[7] = 0.0f;

	data[8] = 0.0f;
	data[9] = 0.0f;
	data[10] = 1.0f;
	data[11] = 0.0f;

	data[12] = translate.x;
	data[13] = translate.y;
	data[14] = 0.0f;
	data[15] = 1.0f;
}

Matrix4 Matrix4::Multiply(const Matrix4& matrix_left, const Matrix4& matrix_right) {
	Matrix4 result_matrix;

	result_matrix.data[0] = matrix_left.data[0] * matrix_right.data[0] + matrix_left.data[1] * matrix_right.data[4] + matrix_left.data[2] * matrix_right.data[8] + matrix_left.data[3] * matrix_right.data[12];
	result_matrix.data[1] = matrix_left.data[0] * matrix_right.data[1] + matrix_left.data[1] * matrix_right.data[5] + matrix_left.data[2] * matrix_right.data[9] + matrix_left.data[3] * matrix_right.data[13];
	result_matrix.data[2] = matrix_left.data[0] * matrix_right.data[2] + matrix_left.data[1] * matrix_right.data[6] + matrix_left.data[2] * matrix_right.data[10] + matrix_left.data[3] * matrix_right.data[14];
	result_matrix.data[3] = matrix_left.data[0] * matrix_right.data[3] + matrix_left.data[1] * matrix_right.data[7] + matrix_left.data[2] * matrix_right.data[11] + matrix_left.data[3] * matrix_right.data[15];

	result_matrix.data[4] = matrix_left.data[4] * matrix_right.data[0] + matrix_left.data[5] * matrix_right.data[4] + matrix_left.data[6] * matrix_right.data[8] + matrix_left.data[7] * matrix_right.data[12];
	result_matrix.data[5] = matrix_left.data[4] * matrix_right.data[1] + matrix_left.data[5] * matrix_right.data[5] + matrix_left.data[6] * matrix_right.data[9] + matrix_left.data[7] * matrix_right.data[13];
	result_matrix.data[6] = matrix_left.data[4] * matrix_right.data[2] + matrix_left.data[5] * matrix_right.data[6] + matrix_left.data[6] * matrix_right.data[10] + matrix_left.data[7] * matrix_right.data[14];
	result_matrix.data[7] = matrix_left.data[4] * matrix_right.data[3] + matrix_left.data[5] * matrix_right.data[7] + matrix_left.data[6] * matrix_right.data[11] + matrix_left.data[7] * matrix_right.data[15];

	result_matrix.data[8] = matrix_left.data[8] * matrix_right.data[0] + matrix_left.data[9] * matrix_right.data[4] + matrix_left.data[10] * matrix_right.data[8] + matrix_left.data[11] * matrix_right.data[12];
	result_matrix.data[9] = matrix_left.data[8] * matrix_right.data[1] + matrix_left.data[9] * matrix_right.data[5] + matrix_left.data[10] * matrix_right.data[9] + matrix_left.data[11] * matrix_right.data[13];
	result_matrix.data[10] = matrix_left.data[8] * matrix_right.data[2] + matrix_left.data[9] * matrix_right.data[6] + matrix_left.data[10] * matrix_right.data[10] + matrix_left.data[11] * matrix_right.data[14];
	result_matrix.data[11] = matrix_left.data[8] * matrix_right.data[3] + matrix_left.data[9] * matrix_right.data[7] + matrix_left.data[10] * matrix_right.data[11] + matrix_left.data[11] * matrix_right.data[15];

	result_matrix.data[12] = matrix_left.data[12] * matrix_right.data[0] + matrix_left.data[13] * matrix_right.data[4] + matrix_left.data[14] * matrix_right.data[8] + matrix_left.data[15] * matrix_right.data[12];
	result_matrix.data[13] = matrix_left.data[12] * matrix_right.data[1] + matrix_left.data[13] * matrix_right.data[5] + matrix_left.data[14] * matrix_right.data[9] + matrix_left.data[15] * matrix_right.data[13];
	result_matrix.data[14] = matrix_left.data[12] * matrix_right.data[2] + matrix_left.data[13] * matrix_right.data[6] + matrix_left.data[14] * matrix_right.data[10] + matrix_left.data[15] * matrix_right.data[14];
	result_matrix.data[15] = matrix_left.data[12] * matrix_right.data[3] + matrix_left.data[13] * matrix_right.data[7] + matrix_left.data[14] * matrix_right.data[11] + matrix_left.data[15] * matrix_right.data[15];

	return result_matrix;
}