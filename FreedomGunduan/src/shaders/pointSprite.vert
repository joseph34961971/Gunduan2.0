#version 430

layout(location = 0) in vec3 direction;
layout(location = 1) in int begin_t;

uniform int time;
uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 p_matrix;
uniform int speed;
uniform int particle_size;
uniform vec3 cameraPos;

void main(void)
{
	vec3 position = direction * ((begin_t + time) * speed % particle_size) / 10.0;

	vec4 new_pos = m_matrix * vec4(position, 1.0);

	float size = 100.0 / distance(cameraPos, vec3(new_pos));

	gl_Position = p_matrix * v_matrix * new_pos;
	gl_PointSize = size;
}