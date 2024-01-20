#include "game_state.h"

#include <cmath>
#include <SDL.h>

Transform::Transform() {
	position = Vector2D(0.0f, 0.0f);
	scale = Vector2D(1.0f, 1.0f);
	rotation_radians = 0.0f;
	transform_matrix = Matrix4D();
}

Vector2D Transform::GetPosition() {
	return position;
}

Vector2D Transform::GetScale() {
	return scale;
}

float Transform::GetRotationInRadians() {
	return rotation_radians;
}

Matrix4D Transform::GetTransformMatrix() {
	return transform_matrix;
}

void Transform::SetPosition(Vector2D position) {
	this->position = position;

	transform_matrix.data[12] = position.x;
	transform_matrix.data[13] = position.y;
}

void Transform::SetScale(Vector2D scale) {
	this->scale = scale;

	transform_matrix.data[0] = cos(rotation_radians) * scale.x;
	transform_matrix.data[1] = -sin(rotation_radians) * scale.x;

	transform_matrix.data[4] = sin(rotation_radians) * scale.y;
	transform_matrix.data[5] = cos(rotation_radians) * scale.y;
}

void Transform::SetRotationInRadians(float rotation_radians) {
	this->rotation_radians = rotation_radians;

	transform_matrix.data[0] = cos(rotation_radians) * scale.x;
	transform_matrix.data[1] = -sin(rotation_radians) * scale.x;

	transform_matrix.data[4] = sin(rotation_radians) * scale.y;
	transform_matrix.data[5] = cos(rotation_radians) * scale.y;
}

unsigned char Tilemap::GetTileByCoordinate(const int x, const int y) {
    int index = x + (y * 40);

    if (index >= 0 && index < 920) {
        return tiles[index];
    }

    return 1;
}

Vector2D Tilemap::TilemapIndexToTilesheetOffset(const int tilemap_index) {
    unsigned char tile = tiles[tilemap_index];

    if (tile == 0) {
        return Vector2D(192.0f, 224.0f);
    }

    int x = tilemap_index % 40;
    int y = tilemap_index / 40;

    bool north      = GetTileByCoordinate(x, y - 1) == 1;
    bool north_east = GetTileByCoordinate(x + 1, y - 1) == 1;
    bool east       = GetTileByCoordinate(x + 1, y) == 1;
    bool south_east = GetTileByCoordinate(x + 1, y + 1) == 1;
    bool south      = GetTileByCoordinate(x, y + 1) == 1;
    bool south_west = GetTileByCoordinate(x - 1, y + 1) == 1;
    bool west       = GetTileByCoordinate(x - 1, y) == 1;
    bool north_west = GetTileByCoordinate(x - 1, y - 1) == 1;

    unsigned char neighbors = 0;

    if (north) {
        neighbors += 1;
    }

    if (north_east && north && east) {
        neighbors += 2;
    }

    if (east) {
        neighbors += 4;
    }

    if (south_east && south && east) {
        neighbors += 8;
    }

    if (south) {
        neighbors += 16;
    }

    if (south_west && south && west) {
        neighbors += 32;
    }

    if (west) {
        neighbors += 64;
    }

    if (north_west && north && west) {
        neighbors += 128;
    }

    Vector2D tilesheet_offset = Vector2D();

    switch (neighbors) {
    case 0:   tilesheet_offset = Vector2D(0.0f, 0.0f); break;
    case 4:   tilesheet_offset = Vector2D(1.0f, 0.0f); break;
    case 92:  tilesheet_offset = Vector2D(2.0f, 0.0f); break;
    case 124: tilesheet_offset = Vector2D(3.0f, 0.0f); break;
    case 116: tilesheet_offset = Vector2D(4.0f, 0.0f); break;
    case 80:  tilesheet_offset = Vector2D(5.0f, 0.0f); break;

    case 16:  tilesheet_offset = Vector2D(0.0f, 1.0f); break;
    case 20:  tilesheet_offset = Vector2D(1.0f, 1.0f); break;
    case 87:  tilesheet_offset = Vector2D(2.0f, 1.0f); break;
    case 223: tilesheet_offset = Vector2D(3.0f, 1.0f); break;
    case 241: tilesheet_offset = Vector2D(4.0f, 1.0f); break;
    case 21:  tilesheet_offset = Vector2D(5.0f, 1.0f); break;
    case 64:  tilesheet_offset = Vector2D(6.0f, 1.0f); break;

    case 29:  tilesheet_offset = Vector2D(0.0f, 2.0f); break;
    case 117: tilesheet_offset = Vector2D(1.0f, 2.0f); break;
    case 85:  tilesheet_offset = Vector2D(2.0f, 2.0f); break;
    case 71:  tilesheet_offset = Vector2D(3.0f, 2.0f); break;
    case 221: tilesheet_offset = Vector2D(4.0f, 2.0f); break;
    case 125: tilesheet_offset = Vector2D(5.0f, 2.0f); break;
    case 112: tilesheet_offset = Vector2D(6.0f, 2.0f); break;

    case 31:  tilesheet_offset = Vector2D(0.0f, 3.0f); break;
    case 253: tilesheet_offset = Vector2D(1.0f, 3.0f); break;
    case 113: tilesheet_offset = Vector2D(2.0f, 3.0f); break;
    case 28:  tilesheet_offset = Vector2D(3.0f, 3.0f); break;
    case 127: tilesheet_offset = Vector2D(4.0f, 3.0f); break;
    case 247: tilesheet_offset = Vector2D(5.0f, 3.0f); break;
    case 209: tilesheet_offset = Vector2D(6.0f, 3.0f); break;

    case 23:  tilesheet_offset = Vector2D(0.0f, 4.0f); break;
    case 199: tilesheet_offset = Vector2D(1.0f, 4.0f); break;
    case 213: tilesheet_offset = Vector2D(2.0f, 4.0f); break;
    case 95:  tilesheet_offset = Vector2D(3.0f, 4.0f); break;
    case 255: tilesheet_offset = Vector2D(4.0f, 4.0f); break;
    case 245: tilesheet_offset = Vector2D(5.0f, 4.0f); break;
    case 81:  tilesheet_offset = Vector2D(6.0f, 4.0f); break;

    case 5:   tilesheet_offset = Vector2D(0.0f, 5.0f); break;
    case 84:  tilesheet_offset = Vector2D(1.0f, 5.0f); break;
    case 93:  tilesheet_offset = Vector2D(2.0f, 5.0f); break;
    case 119: tilesheet_offset = Vector2D(3.0f, 5.0f); break;
    case 215: tilesheet_offset = Vector2D(4.0f, 5.0f); break;
    case 193: tilesheet_offset = Vector2D(5.0f, 5.0f); break;
    case 17:  tilesheet_offset = Vector2D(6.0f, 5.0f); break;

    case 1:   tilesheet_offset = Vector2D(1.0f, 6.0f); break;
    case 7:   tilesheet_offset = Vector2D(2.0f, 6.0f); break;
    case 197: tilesheet_offset = Vector2D(3.0f, 6.0f); break;
    case 69:  tilesheet_offset = Vector2D(4.0f, 6.0f); break;
    case 68:  tilesheet_offset = Vector2D(5.0f, 6.0f); break;
    case 65:  tilesheet_offset = Vector2D(6.0f, 6.0f); break;
    }

    tilesheet_offset.y = 6.0f - tilesheet_offset.y + 1.0f;
    tilesheet_offset.x *= 32.0f;
    tilesheet_offset.y *= 32.0f;

    return tilesheet_offset;
}

bool Tilemap::IsPointInWall(const Vector2D point) {
    float tile_x = point.x / 32.0f;
    float tile_y = point.y / 32.0f;

    int index = (int)floor(tile_x) + (floor(tile_y) * 40);

    return tiles[index];
}