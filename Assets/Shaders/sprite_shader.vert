#version 330

uniform mat4 transform_view;
uniform vec4 sprite_size_and_offset;

uniform sampler2D sprite_texture;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

out vec2 frag_uv;

void main()
{
	vec4 position = vec4(in_position, 1.0);

	gl_Position = position * transform_view;

	ivec2 sprite_sheet_size = textureSize(sprite_texture, 0);

	vec2 normalized_sprite_size = vec2(sprite_size_and_offset.x / sprite_sheet_size.x, sprite_size_and_offset.y / sprite_sheet_size.y);
	vec2 normalized_sprite_offset = vec2(sprite_size_and_offset.z / sprite_sheet_size.x, (sprite_sheet_size.y - sprite_size_and_offset.y - sprite_size_and_offset.w) / sprite_sheet_size.y);

	frag_uv = (in_uv * normalized_sprite_size) + normalized_sprite_offset;
}
