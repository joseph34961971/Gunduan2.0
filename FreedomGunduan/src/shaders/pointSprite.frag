#version 430

out vec4 color;

uniform sampler2D tex_particle;

void main(void)
{
	color = texture(tex_particle, gl_PointCoord);
	if (color.a < 0.1)
		discard;
}