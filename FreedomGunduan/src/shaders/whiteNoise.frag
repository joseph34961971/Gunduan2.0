#version 430 core
out vec4 f_color;

in V_OUT
{
   vec2 uv;
} f_in;

uniform float u_time;

float random2d(vec2 coord)
{
	return fract(sin(dot(coord.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
	vec2 coord = f_in.uv.xy;
	vec3 color = vec3(0.0);
	float grain = 0.0;

	grain = random2d(vec2(sin(coord)) * u_time);

	color = vec3(grain);

	f_color = vec4(color, 1.0);
}