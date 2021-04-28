#version 430 core
#define MASK_SIZE 9
out vec4 f_color;

in V_OUT
{
   vec2 uv;
} f_in;

uniform sampler2D screen;
uniform vec2 img_size;

void main()
{
    vec4 color;
    int half_size = int(MASK_SIZE / 2);
    int mask[MASK_SIZE];
    mask[0] = 1;
    for (int i = 1; i < MASK_SIZE; i++)
    {
        int a = MASK_SIZE - 1;
        int b = 1;
        int tmp_a = a;
        int tmp_b = b;
        for (int j = 1; j < i; j++)
        {
            a = a * (tmp_a - j);
            b = b * (tmp_b + j);
        }
        mask[i] = a / b;
    }
    for (int i = -half_size; i <= half_size; ++i)
    {
        for (int j = -half_size; j <= half_size; ++j)
        {
            vec4 c = texture(screen, f_in.uv + vec2(i, j) / img_size) * mask[i+ half_size] * mask[j+ half_size];
            color += (c / (pow(2, MASK_SIZE - 1) * pow(2, MASK_SIZE - 1)));
        }
    }
    f_color = color;
}