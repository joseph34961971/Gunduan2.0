#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 u_model;

layout (std140, binding = 0) uniform commom_matrices
{
    mat4 u_view;
    mat4 u_projection;
};

out V_OUT
{
   vec3 position;
   vec3 normal;
   vec3 view_pos;
} v_out;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);

    v_out.position = vec3(u_model * vec4(position, 1.0f));
    v_out.normal = mat3(transpose(inverse(u_model))) * normal;

    // Get vertex position in eye coordinates
    vec4 vPosition4 = u_view * u_model * vec4(position, 1);
    v_out.view_pos = vPosition4.xyz / vPosition4.w;
}