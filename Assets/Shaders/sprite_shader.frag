#version 330

in vec2 frag_uv;

out vec4 out_color;

uniform sampler2D sprite_texture;

void main()
{
	out_color = texture(sprite_texture, frag_uv);
}
