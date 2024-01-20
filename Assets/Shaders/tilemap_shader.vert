#version 460

uniform mat4 transform_view;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;

out vec2 frag_uv;

void main()
{
	vec4 position = vec4(in_position, 1.0);

	gl_Position = position * transform_view;

	frag_uv = in_uv;
}
