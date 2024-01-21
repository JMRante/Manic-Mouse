#pragma once
#ifndef MANICMOUSE_DATA_LOADER_H_
#define MANICMOUSE_DATA_LOADER_H_

#include <string>

#include <vector>

#include "renderer.h"

enum LevelParserState {
	ParseStart,
	ParseCheese,
	ParseRedKey,
	ParseYellowKey,
	ParseBlueKey,
	ParseRedDoor,
	ParseYellowDoor,
	ParseBlueDoor,
	ParseMovingBlocksCount,
	ParseMovingBlocks,
	ParseTileSheet,
	ParseTileMap
};

class DataLoader
{
public:
	static std::string LoadText(const std::string& text_file_path);
	static ShaderProgram LoadShader(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path);
	static Texture LoadPNG(const std::string& png_file_path);
	static void LoadConfig(const std::string& config_file_path);
	static void LoadLevels(const std::string& levels_file_path, std::vector<LevelState*>& levels);
private:
	static void FlipSurface(SDL_Surface* surface);
};

#endif