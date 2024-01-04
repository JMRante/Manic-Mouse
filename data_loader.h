#pragma once
#ifndef MANICMOUSE_DATA_LOADER_H_
#define MANICMOUSE_DATA_LOADER_H_

#include <string>

#include "renderer.h"

class DataLoader
{
public:
	static std::string LoadText(const std::string& text_file_path);
	static ShaderProgram LoadShader(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path);
	static void LoadPNG(const std::string& png_file_path);
	static void LoadConfig(const std::string& config_file_path);
	static void LoadLevel(const std::string& level_file_path);
};

#endif