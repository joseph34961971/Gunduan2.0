#version 430 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coordinate;

uniform mat4 u_model;
uniform vec3 vLightPosition;

layout (std140, binding = 0) uniform commom_matrices
{
    mat4 u_view;
    mat4 u_projection;
};

out V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
   vec3 viewDir;
} v_out;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(position, 1.0f);

    v_out.position = vec3(u_model * vec4(position, 1.0f));
    v_out.normal = mat3(transpose(inverse(u_model))) * normal;
    v_out.texture_coordinate = vec2(texture_coordinate.x, 1.0 - texture_coordinate.y);

    // Get vertex position in eye coordinates
    vec4 vPosition4 = u_view * u_model * vec4(position, 1);
    vec3 vPosition3 = vPosition4.xyz / vPosition4.w;

    // Get vector to light source
    v_out.viewDir = normalize(vLightPosition - vPosition3);
}