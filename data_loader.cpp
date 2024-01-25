#include "data_loader.h"

#include <SDL.h>
#include <SDL_image.h>
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>

#include "renderer.h"
#include "game_state.h"

std::string DataLoader::LoadText(const std::string& text_file_path) {
	std::ifstream file_input_stream;
	file_input_stream.open(text_file_path);

	if (file_input_stream.is_open()) {
		std::stringstream string_stream;
		string_stream << file_input_stream.rdbuf();
		return string_stream.str();
	}

	return "";
}

ShaderProgram DataLoader::LoadShader(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path) {
	ShaderProgram new_shader;

	std::string vertex_shader_src_str = DataLoader::LoadText(vertex_shader_file_path);
	std::string fragment_shader_src_str = DataLoader::LoadText(fragment_shader_file_path);

	const GLchar* vertex_shader_src = vertex_shader_src_str.c_str();
	const GLchar* fragment_shader_src = fragment_shader_src_str.c_str();

	new_shader.vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(new_shader.vertex_shader_id, 1, &vertex_shader_src, nullptr);
	glCompileShader(new_shader.vertex_shader_id);
	GLint is_vertex_shader_compiled = 0;
	glGetShaderiv(new_shader.vertex_shader_id, GL_COMPILE_STATUS, &is_vertex_shader_compiled);

	if (is_vertex_shader_compiled == GL_FALSE) {
		char buffer[256];
		memset(buffer, 0, 256);
		glGetShaderInfoLog(new_shader.vertex_shader_id, 255, nullptr, buffer);
		SDL_Log("Failed to compile vertex shader: %s", buffer);

		return new_shader;
	}

	new_shader.fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(new_shader.fragment_shader_id, 1, &fragment_shader_src, nullptr);
	glCompileShader(new_shader.fragment_shader_id);
	GLint is_fragment_shader_compiled = 0;
	glGetShaderiv(new_shader.fragment_shader_id, GL_COMPILE_STATUS, &is_fragment_shader_compiled);

	if (is_fragment_shader_compiled == GL_FALSE) {
		char buffer[256];
		memset(buffer, 0, 256);
		glGetShaderInfoLog(new_shader.fragment_shader_id, 255, nullptr, buffer);
		SDL_Log("Failed to compile fragment shader: %s", buffer);

		return new_shader;
	}

	new_shader.id = glCreateProgram();
	glAttachShader(new_shader.id, new_shader.vertex_shader_id);
	glAttachShader(new_shader.id, new_shader.fragment_shader_id);
	glLinkProgram(new_shader.id);
	GLint is_shader_program_compiled;
	glGetProgramiv(new_shader.id, GL_LINK_STATUS, &is_shader_program_compiled);

	if (is_shader_program_compiled == GL_FALSE)
	{
		char buffer[256];
		memset(buffer, 0, 256);
		glGetProgramInfoLog(new_shader.id, 255, nullptr, buffer);
		SDL_Log("Failed to compile shader program: %s", buffer);

		return new_shader;
	}

	return new_shader;
}

Texture DataLoader::LoadPNG(const std::string& png_file_path) {
	SDL_Surface* png_surface = IMG_Load(png_file_path.c_str());
	FlipSurface(png_surface);

	Texture new_sprite;
	new_sprite.width = png_surface->w;
	new_sprite.height = png_surface->h;

	glGenTextures(1, &new_sprite.id);
	glBindTexture(GL_TEXTURE_2D, new_sprite.id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, png_surface->w, png_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, png_surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	SDL_FreeSurface(png_surface);

	return new_sprite;
}

void DataLoader::LoadConfig(const std::string& config_file_path) {

}

void DataLoader::LoadLevels(const std::string& levels_file_path, std::vector<LevelState*>& levels) {
	std::ifstream file_input_stream;
	file_input_stream.open(levels_file_path);

	if (file_input_stream.is_open()) {
		std::string line;
		LevelParserState parser_state = ParseStart;
		int object_counter = 0;
		int property_counter = 0;
		LevelState* level = new LevelState();

		while (std::getline(file_input_stream, line)) {
			switch (parser_state) {
			case ParseStart:
				switch (property_counter) {
				case 0: level->start.x = std::stof(line) + 16.0f; property_counter++; break;
				case 1: level->start.y = std::stof(line) - 16.0f; property_counter = 0; parser_state = ParseCheese; break;
				}
				break;
			case ParseCheese:
				switch (property_counter) {
				case 0: level->cheese.transform.SetPositionX(std::stof(line) + 16.0f); property_counter++; break;
				case 1: level->cheese.transform.SetPositionY(std::stof(line) - 16.0f); property_counter = 0; parser_state = ParseRedKey; break;
				}
				break;
			case ParseRedKey:
				switch (property_counter) {
				case 0: level->red_key.active = (bool)std::stoi(line); property_counter++; break;
				case 1: level->red_key.transform.SetPositionX(std::stof(line) + 16.0f); property_counter++; break;
				case 2: level->red_key.transform.SetPositionY(std::stof(line) - 16.0f); property_counter = 0; parser_state = ParseYellowKey; break;
				}
				break;
			case ParseYellowKey:
				switch (property_counter) {
				case 0: level->yellow_key.active = (bool)std::stoi(line); property_counter++; break;
				case 1: level->yellow_key.transform.SetPositionX(std::stof(line) + 16.0f); property_counter++; break;
				case 2: level->yellow_key.transform.SetPositionY(std::stof(line) - 16.0f); property_counter = 0; parser_state = ParseBlueKey; break;
				}
				break;
			case ParseBlueKey:
				switch (property_counter) {
				case 0: level->blue_key.active = (bool)std::stoi(line); property_counter++; break;
				case 1: level->blue_key.transform.SetPositionX(std::stof(line) + 16.0f); property_counter++; break;
				case 2: level->blue_key.transform.SetPositionY(std::stof(line) - 16.0f); property_counter = 0; parser_state = ParseRedDoor; break;
				}
				break;
			case ParseRedDoor:
				switch (property_counter) {
				case 0: level->red_door.active = (bool)std::stoi(line); property_counter++; break;
				case 1: level->red_door.transform.SetPositionX(std::stof(line) + 16.0f); property_counter++; break;
				case 2: level->red_door.transform.SetPositionY(std::stof(line) - 16.0f); property_counter = 0; parser_state = ParseYellowDoor; break;
				}
				break;
			case ParseYellowDoor:
				switch (property_counter) {
				case 0: level->yellow_door.active = (bool)std::stoi(line); property_counter++; break;
				case 1: level->yellow_door.transform.SetPositionX(std::stof(line) + 16.0f); property_counter++; break;
				case 2: level->yellow_door.transform.SetPositionY(std::stof(line) - 16.0f); property_counter = 0; parser_state = ParseBlueDoor; break;
				}
				break;
			case ParseBlueDoor:
				switch (property_counter) {
				case 0: level->blue_door.active = (bool)std::stoi(line); property_counter++; break;
				case 1: level->blue_door.transform.SetPositionX(std::stof(line) + 16.0f); property_counter++; break;
				case 2: level->blue_door.transform.SetPositionY(std::stof(line) - 16.0f); property_counter = 0; parser_state = ParseMovingBlocksCount; break;
				}
				break;
			case ParseMovingBlocksCount:
				level->moving_block_count = std::stoi(line);
				parser_state = level->moving_block_count > 0 ? ParseMovingBlocks : ParseTileSheet;
				break;
			case ParseMovingBlocks:
				if (object_counter < level->moving_block_count) {
					MovingBlock& moving_block = level->moving_blocks[object_counter];
					switch (property_counter) {
						case 0: moving_block.behavior = (MovingBlockBehavior)std::stoi(line); property_counter++; break;
						case 1: moving_block.transform.SetPositionX(std::stof(line) + 16.0f); property_counter++; break;
						case 2: moving_block.transform.SetPositionY(std::stof(line) - 16.0f); property_counter = 0; object_counter++;  break;
					}
					break;
				} else {
					parser_state = ParseTileSheet;
					object_counter = 0;
				}
			case ParseTileSheet:
				level->tilemap.tilesheet_index = std::stoi(line);
				parser_state = ParseTileMap;
				break;
			case ParseTileMap:
				int i = 0;

				for (int i = 0; i < 920; i++) {
					level->tilemap.tiles[i] = std::stoi(line.substr(i, 1));
				}

				levels.push_back(level);
				level = new LevelState();

				parser_state = ParseStart;
			}
		}
	}
}

// From: https://stackoverflow.com/questions/65815332/flipping-a-surface-vertically-in-sdl2
void DataLoader::FlipSurface(SDL_Surface* surface)
{
	SDL_LockSurface(surface);

	int pitch = surface->pitch;
	char* temp = new char[pitch];
	char* pixels = (char*)surface->pixels;

	for (int i = 0; i < surface->h / 2; ++i) {
		char* row1 = pixels + i * pitch;
		char* row2 = pixels + (surface->h - i - 1) * pitch;

		memcpy(temp, row1, pitch);
		memcpy(row1, row2, pitch);
		memcpy(row2, temp, pitch);
	}

	delete[] temp;

	SDL_UnlockSurface(surface);
}