// ADS Point lighting Shader
// Fragment Shader
// Richard S. Wright Jr.
// OpenGL SuperBible
#version 430
struct MaterialInfo{
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
};

uniform MaterialInfo Material;
out vec4 vFragColor;

in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;

void main(void)
{ 
    vec3 color1 = Material.Kd;
    float color2;

    float intensity = dot(normalize(vVaryingLightDir), normalize(vVaryingNormal));

    if (intensity > 0.95)      color2 = 1.0;
    else if (intensity > 0.75) color2 = 0.8;
    else if (intensity > 0.50) color2 = 0.6;
    else if (intensity > 0.25) color2 = 0.4;
    else                       color2 = 0.2;

    vFragColor = vec4(color1 * color2, 1.0);
}