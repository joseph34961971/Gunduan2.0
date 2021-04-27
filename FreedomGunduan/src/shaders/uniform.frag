#version 430 core
out vec4 f_color;

in V_OUT
{
   vec2 uv;
} f_in;

uniform sampler2D screen;

float uniformRG(float x)
{
    float neer_color[] = { 0, 36 / 256.0, 73 / 256.0, 109 / 256.0, 146 / 256.0, 182 / 256.0, 219 / 256.0, 1.0};
    int neer_color_num = 8;
    for (int neer_color_index = 0; neer_color_index < neer_color_num - 1; neer_color_index++)
    {
        if (neer_color[neer_color_index] <= x && x < neer_color[neer_color_index + 1])
        {
            if (x - neer_color[neer_color_index] < neer_color[neer_color_index + 1] - x)
            {
                return neer_color[neer_color_index];
            }
            else
            {
                return neer_color[neer_color_index + 1];
            }
        }
    }
    return neer_color[neer_color_num - 1];
}

float uniformB(float x)
{
    float neer_color[] = { 0, 85 / 256.0, 170 / 256.0, 1.0 };
    int neer_color_num = 4;
    for (int neer_color_index = 0; neer_color_index < neer_color_num - 1; neer_color_index++)
    {
        if (neer_color[neer_color_index] <= x && x < neer_color[neer_color_index + 1])
        {
            if (x - neer_color[neer_color_index] < neer_color[neer_color_index + 1] - x)
            {
                return neer_color[neer_color_index];
            }
            else
            {
                return neer_color[neer_color_index + 1];
            }
        }
    }
    return neer_color[neer_color_num - 1];
}

void main()
{
    vec4 color = texture(screen, f_in.uv);
    f_color = vec4(uniformRG(color.r), uniformRG(color.g), uniformB(color.b), 1.0);
}