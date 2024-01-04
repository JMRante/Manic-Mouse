#include "data_loader.h"

#include <SDL.h>
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>

#include "renderer.h"

// Data Loaders (allocate memory for resources on load, then either release when moved to GPU or release on quit)
// Text Files
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

// Shader Files
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

	new_shader.shader_program_id = glCreateProgram();
	glAttachShader(new_shader.shader_program_id, new_shader.vertex_shader_id);
	glAttachShader(new_shader.shader_program_id, new_shader.fragment_shader_id);
	glLinkProgram(new_shader.shader_program_id);
	GLint is_shader_program_compiled;
	glGetProgramiv(new_shader.shader_program_id, GL_LINK_STATUS, &is_shader_program_compiled);

	if (is_shader_program_compiled == GL_FALSE)
	{
		char buffer[256];
		memset(buffer, 0, 256);
		glGetProgramInfoLog(new_shader.shader_program_id, 255, nullptr, buffer);
		SDL_Log("Failed to compile shader program: %s", buffer);

		return new_shader;
	}

	return new_shader;
}

// PNG Files
void DataLoader::LoadPNG(const std::string& png_file_path) {

}

// Config Files
void DataLoader::LoadConfig(const std::string& config_file_path) {

}

// Level Files
void DataLoader::LoadLevel(const std::string& level_file_path) {

}