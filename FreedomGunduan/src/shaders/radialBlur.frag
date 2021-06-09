#version 430 core
out vec4 f_color;

in V_OUT
{
   vec2 uv;
} f_in;

uniform sampler2D screen;
// url: https://stackoverflow.com/questions/4579020/how-do-i-use-a-glsl-shader-to-apply-a-radial-blur-to-an-entire-scene
const float sampleDist = 0.4;
const float sampleStrength = 2.2;

void main()
{
    float samples[10];
    samples[0] = -0.08;
    samples[1] = -0.05;
    samples[2] = -0.03;
    samples[3] = -0.02;
    samples[4] = -0.01;
    samples[5] =  0.01;
    samples[6] =  0.02;
    samples[7] =  0.03;
    samples[8] =  0.05;
    samples[9] =  0.08;

    vec2 dir = 0.5 - f_in.uv; 
    float dist = sqrt(dir.x * dir.x + dir.y * dir.y); 
    dir = dir / dist; 

    vec4 color = texture(screen, f_in.uv);
    vec4 sum = color;

    for (int i = 0; i < 10; i++)
        sum += texture(screen, f_in.uv + dir * samples[i] * sampleDist );

    sum *= 1.0 / 11.0;
    float t = dist * sampleStrength;
    t = clamp(t, 0.0, 1.0);
    f_color = vec4(mix(color, sum, t).rgb, 1.0);
}