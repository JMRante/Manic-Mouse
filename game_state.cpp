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
	transform_matrix.data[13] = -position.y;
}

void Transform::SetPositionX(float x) {
    this->position.x = x;

    transform_matrix.data[12] = x;
}

void Transform::SetPositionY(float y) {
    this->position.y = y;

    transform_matrix.data[13] = -y;
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

LevelState::LevelState() {
    mouse.active = true;
    mouse.sprites[0].size = { 32.0f, 32.0f };
    mouse.sprites[0].offset = { 0.0f, 0.0f };
    mouse.sprites[1].size = { 32.0f, 32.0f };
    mouse.sprites[1].offset = { 32.0f, 0.0f };
    mouse.sprites[2].size = { 32.0f, 32.0f };
    mouse.sprites[2].offset = { 64.0f, 0.0f };
    mouse.sprites[3].size = { 32.0f, 32.0f };
    mouse.sprites[3].offset = { 0.0f, 32.0f };
    mouse.sprite_index = 0;
    mouse.sprite_speed = 0.12f;
    mouse.transform = Transform();
    mouse.transform.SetScale(mouse.sprites[0].size);
    mouse.is_dead = false;

    cheese.active = true;
    cheese.sprite.size = { 32.0f, 32.0f };
    cheese.sprite.offset = { 96.0f, 0.0f };
    cheese.transform = Transform();
    cheese.transform.SetScale(cheese.sprite.size);

    red_key.active = false;
    red_key.sprite.size = { 32.0f, 32.0f };
    red_key.sprite.offset = { 0.0f, 96.0f };
    red_key.transform = Transform();
    red_key.transform.SetScale(red_key.sprite.size);
    red_key.color = Red;
    red_key.collected = false;

    yellow_key.active = false;
    yellow_key.sprite.size = { 32.0f, 32.0f };
    yellow_key.sprite.offset = { 32.0f, 96.0f };
    yellow_key.transform = Transform();
    yellow_key.transform.SetScale(yellow_key.sprite.size);
    yellow_key.color = Yellow;
    yellow_key.collected = false;

    blue_key.active = false;
    blue_key.sprite.size = { 32.0f, 32.0f };
    blue_key.sprite.offset = { 64.0f, 96.0f };
    blue_key.transform = Transform();
    blue_key.transform.SetScale(blue_key.sprite.size);
    blue_key.color = Blue;
    blue_key.collected = false;

    red_door.active = false;
    red_door.sprite.size = { 32.0f, 32.0f };
    red_door.sprite.offset = { 0.0f, 64.0f };
    red_door.transform = Transform();
    red_door.transform.SetScale(red_door.sprite.size);
    red_door.color = Red;

    yellow_door.active = false;
    yellow_door.sprite.size = { 32.0f, 32.0f };
    yellow_door.sprite.offset = { 32.0f, 64.0f };
    yellow_door.transform = Transform();
    yellow_door.transform.SetScale(yellow_door.sprite.size);
    yellow_door.color = Yellow;

    blue_door.active = false;
    blue_door.sprite.size = { 32.0f, 32.0f };
    blue_door.sprite.offset = { 64.0f, 64.0f };
    blue_door.transform = Transform();
    blue_door.transform.SetScale(blue_door.sprite.size);
    blue_door.color = Blue;

    moving_block_count = 0;

    for (int i = 0; i < 20; i++) {
        MovingBlock& moving_block = moving_blocks[i];
        moving_block.active = false;
        moving_block.sprite.size = { 32.0f, 32.0f };
        moving_block.sprite.offset = { 32.0f, 32.0f };
        moving_block.transform = Transform();
        moving_block.transform.SetScale(moving_block.sprite.size);
        moving_block.behavior = Horizontal;
    }
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

    int index = (int)floor(tile_x) + ((int)floor(tile_y) * 40);

    return tiles[index];
}