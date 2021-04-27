#version 430 core
out vec4 f_color;

in V_OUT
{
   vec2 uv;
} f_in;

uniform sampler2D screen;

vec3 gray(vec3 color)
{
    return vec3(color.r * 0.3 + color.g * 0.59 + color.b * 0.11);
}

void main()
{
    vec4 color = vec4(gray(texture(screen, f_in.uv).rgb), 1);
    f_color = color;
}