#include "game_state.h"

#include <cmath>

Transform::Transform() {
	position = Vector2(0.0f, 0.0f);
	scale = Vector2(1.0f, 1.0f);
	rotation_radians = 0.0f;
	transform_matrix = Matrix4();
}

Vector2 Transform::GetPosition() {
	return position;
}

Vector2 Transform::GetScale() {
	return scale;
}

float Transform::GetRotationInRadians() {
	return rotation_radians;
}

Matrix4 Transform::GetTransformMatrix() {
	return transform_matrix;
}

void Transform::SetPosition(Vector2 position) {
	this->position = position;
	transform_matrix.data[12] = position.x;
	transform_matrix.data[13] = position.y;
}

void Transform::SetScale(Vector2 scale) {
	this->scale = scale;
	transform_matrix.data[0] = cos(rotation_radians) * scale.x;
	transform_matrix.data[5] = cos(rotation_radians) * scale.y;
}

void Transform::SetRotationInRadians(float rotation) {
	transform_matrix.data[0] = cos(rotation_radians) * scale.x;
	transform_matrix.data[1] = -sin(rotation_radians);

	transform_matrix.data[4] = sin(rotation_radians);
	transform_matrix.data[5] = cos(rotation_radians) * scale.y;
}