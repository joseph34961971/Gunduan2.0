#version 430 core

#extension GL_NV_shadow_samplers_cube : enable

out vec4 f_color;

in V_OUT
{
   vec3 position;
   vec3 normal;
   vec3 view_pos;
} f_in;

uniform samplerCube skybox;

uniform int mode;

uniform float Eta = 1 / 2.42; //Diamond

void main()
{
    vec3 InVec = normalize(f_in.position - f_in.view_pos); 
    //湖面（各頂點）到眼睛的入射向量
 
    vec3 ReflectVec = reflect(InVec, f_in.normal);
    vec3 RefractVec;
    float cosTheta = dot(InVec, f_in.normal) / length(InVec) / length(f_in.normal);
    if (cosTheta > 0)
        RefractVec = refract(InVec, -f_in.normal, 2.42); //Eta，折射率，2.42
    else
        RefractVec = refract(InVec, f_in.normal, Eta); //Eta，折射率，1 / 2.42

    vec3 ReflectColor = vec3(texture(skybox, ReflectVec));
    vec3 RefractColor = vec3(texture(skybox, RefractVec));

    vec3 color;
    float ratio;
    if (mode == 0)
    {
        color = vec3(texture(skybox, ReflectVec));
    }
    else if (mode == 1)
    {
        color = vec3(texture(skybox, RefractVec));
    }
    else if (mode == 2)
    {
        ratio = 0.4 + 0.6 * pow(min(0.0, 1.0 - dot(-InVec, f_in.normal)), 4.0); //float fresnel 

        color = mix(RefractColor, ReflectColor, ratio);
    }
    
    f_color =  vec4(color, 1.0);
}