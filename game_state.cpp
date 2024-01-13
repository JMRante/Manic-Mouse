#include "game_state.h"

#include <cmath>

Transform::Transform() {
	x = 0.0f;
	y = 0.0f;
	scale_x = 1.0f;
	scale_y = 1.0f;
	rotation_radians = 0.0f;

	transform_matrix[0] = 1.0f;
	transform_matrix[1] = 0.0f;
	transform_matrix[2] = 0.0f;
	transform_matrix[3] = 0.0f;

	transform_matrix[4] = 0.0f;
	transform_matrix[5] = 1.0f;
	transform_matrix[6] = 0.0f;
	transform_matrix[7] = 0.0f;

	transform_matrix[8] = 0.0f;
	transform_matrix[9] = 0.0f;
	transform_matrix[10] = 1.0f;
	transform_matrix[11] = 0.0f;

	transform_matrix[12] = 0.0f;
	transform_matrix[13] = 0.0f;
	transform_matrix[14] = 0.0f;
	transform_matrix[15] = 1.0f;
}

float Transform::GetX() {
	return x;
}

float Transform::GetY() {
	return y;
}

float Transform::GetScaleX() {
	return scale_x;
}

float Transform::GetScaleY() {
	return scale_y;
}

float Transform::GetRotationInRadians() {
	return rotation_radians;
}

float* Transform::GetTransformMatrix() {
	return transform_matrix;
}

void Transform::SetX(float x) {
	this->x = x;
	transform_matrix[12] = x;
}

void Transform::SetY(float y) {
	this->y = y;
	transform_matrix[13] = y;
}

void Transform::SetScaleX(float scale_x) {
	this->scale_x = scale_x;
	transform_matrix[0] = cos(rotation_radians) * scale_x;
}

void Transform::SetScaleY(float scale_y) {
	this->scale_y = scale_y;
	transform_matrix[5] = cos(rotation_radians) * scale_y;
}

void Transform::SetRotationInRadians(float rotation) {
	transform_matrix[0] = cos(rotation_radians) * scale_x;
	transform_matrix[1] = -sin(rotation_radians);

	transform_matrix[4] = sin(rotation_radians);
	transform_matrix[5] = cos(rotation_radians) * scale_y;
}