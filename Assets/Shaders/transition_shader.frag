#version 330

in vec2 frag_uv;

out vec4 out_color;

uniform vec2 focus_point;
uniform float focus_radius;

void main()
{
	vec2 distance_from_focus_point = focus_point - gl_FragCoord.xy;
	float inside_circle = float(length(distance_from_focus_point) > focus_radius);

	out_color = vec4(0.0f, 0.0f, 0.0f, inside_circle);
}
