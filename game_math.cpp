#include "game_math.h"

#include <cmath>

Vector2D operator+(const Vector2D& vector_left, const Vector2D& vector_right) {
	return Vector2D(vector_left.x + vector_right.x, vector_left.y + vector_right.y);
}

Vector2D operator-(const Vector2D& vector_left, const Vector2D& vector_right) {
	return Vector2D(vector_left.x - vector_right.x, vector_left.y - vector_right.y);
}

Vector2D operator*(const Vector2D& vector_left, const Vector2D& vector_right) {
	return Vector2D(vector_left.x * vector_right.x, vector_left.y * vector_right.y);
}

Vector2D operator*(const Vector2D& vector, float scalar) {
	return Vector2D(vector.x * scalar, vector.y * scalar);
}

Vector2D operator*(float scalar, const Vector2D& vector) {
	return Vector2D(vector.x * scalar, vector.y * scalar);
}

Vector2D& Vector2D::operator*=(float scalar) {
	x *= scalar;
	y *= scalar;
	return *this;
}

Vector2D& Vector2D::operator+=(const Vector2D& vector_right) {
	x += vector_right.x;
	y += vector_right.y;
	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& vector_right) {
	x -= vector_right.x;
	y -= vector_right.y;
	return *this;
}

float Vector2D::Length() const {
	return sqrt(x * x + y * y);
}

float Vector2D::LengthSquared() const {
	return x * x + y * y;
}

float Vector2D::Dot(const Vector2D& vector_left, const Vector2D& vector_right) {
	return (vector_left.x * vector_right.x) + (vector_left.y * vector_right.y);
}

float Vector2D::Angle(const Vector2D& vector_left, const Vector2D& vector_right) {
	return acos(Vector2D::Dot(vector_left, vector_right) / (vector_left.Length() * vector_right.Length()));
}

Vector2D Vector2D::Normalized(const Vector2D& vector)
{
	Vector2D normalized_vector = vector;
	float length = vector.Length();
	normalized_vector.x /= length;
	normalized_vector.y /= length;
	return normalized_vector;
}

const Vector2D Vector2D::zero = { 0.0f, 0.0f };
const Vector2D Vector2D::right = { 1.0f, 0.0f };
const Vector2D Vector2D::up = { 0.0f, 1.0f };

Matrix4D::Matrix4D() {
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

Matrix4D::Matrix4D(Vector2D translate, float rotate_radians_on_z, Vector2D scale) {
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

Matrix4D operator*(const Matrix4D& matrix_left, const Matrix4D& matrix_right) {
	Matrix4D result_matrix;

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

float Lerp(float start, float end, float t) {
	return (1 - t) * start + t * end;
}

Vector2D LerpVector2D(const Vector2D& start, const Vector2D& end, float t) {
	return Vector2D(Lerp(start.x, end.x, t), Lerp(start.y, end.y, t));
}

bool IsPointCollidingWithCircle(const Vector2D& point, const Vector2D& circle_origin, float circle_radius) {
	Vector2D between = point - circle_origin;

	if (between.LengthSquared() > circle_radius * circle_radius) {
		return false;
	} else {
		return true;
	}
}

bool IsPointCollidingWithTileArray(const Vector2D& point, unsigned char* tile_data) {
	int point_x_index = (int)floor(point.x / 32.0f);
	int point_y_index = (int)floor(point.y / 32.0f);
	int tile_index = point_x_index + (point_y_index * 40);

	if (tile_data[tile_index] == 1) {
		return true;
	} else {
		return false;
	}
}

bool IsContinuousPointCollidingWithTileArray(const Vector2D& previous_point, const Vector2D& current_point, unsigned char* tile_data, Vector2D& collision_point) {
	Vector2D path = current_point - previous_point;
	int segment_count = (int)ceil(path.Length() / 32.0f);

	for (int i = 0; i <= segment_count; i++) {
		Vector2D point_to_check = current_point;

		if (segment_count != 0) {
			point_to_check = LerpVector2D(previous_point, current_point, i / (float)segment_count);
		}

		if (IsPointCollidingWithTileArray(point_to_check, tile_data)) {
			collision_point = point_to_check;
			return true;
		}
	}

	return false;
}

bool IsPointCollidingWithAABB(const Vector2D& point, const Vector2D& aabb_origin, const Vector2D& aabb_half_extents) {
	float x1 = aabb_origin.x - aabb_half_extents.x;
	float x2 = aabb_origin.x + aabb_half_extents.x;

	float y1 = aabb_origin.y - aabb_half_extents.y;
	float y2 = aabb_origin.y + aabb_half_extents.y;

	return point.x > x1 && point.x < x2 && point.y > y1 && point.y < y2;
}

bool IsContinuousPointCollidingWithAABB(const Vector2D& previous_point, const Vector2D& current_point, const Vector2D& aabb_origin, const Vector2D& aabb_half_extents, Vector2D& collision_point) {
	Vector2D path = current_point - previous_point;
	int segment_count = (int)ceil(path.Length() / 32.0f);

	for (int i = 0; i <= segment_count; i++) {
		Vector2D point_to_check = current_point;

		if (segment_count != 0) {
			point_to_check = LerpVector2D(previous_point, current_point, i / (float)segment_count);
		}

		if (IsPointCollidingWithAABB(point_to_check, aabb_origin, aabb_half_extents)) {
			collision_point = point_to_check;
			return true;
		}
	}

	return false;
}