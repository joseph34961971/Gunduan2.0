#version 430 core
out vec4 f_color;

in V_OUT
{
   vec2 uv;
} f_in;

uniform sampler2D screen;

uniform vec2 img_size;

float dx = 1.0/1024.0, dy = 1.0/1024.0;

void main()
{   
    float rows = img_size.x;
    float cols = img_size.y;
    float nx = 1.0 / cols, ny = 1.0 / rows;
    vec2 coord;
    coord[0] = floor(f_in.uv.x * cols) / cols;
    coord[1] = floor(f_in.uv.y * rows) / rows;
    float l = coord[0] - nx / 0.5, bot = coord[1] - ny / 0.5;
    float r = l + nx, top = bot + ny;
    l = max(l, 0.0); bot = max(bot, 0.0);
    r = min(r, 1.0); top = min(top, 1.0);
    //float w = 1.0 / ((r - l) / dx + 1) / ((top - bot) / dx + 1);
    vec3 texcolor = vec3(0.0);
    for(float x = l;x<=r;x+=dx){
        for(float y = bot;y <= top;y+=dy){
            texcolor += texture(screen, vec2(x,y)).rgb * 0.25;
        }
    }
    f_color = vec4(texcolor, 1.0);
}

//uniform float u_time;
//
//float random2d(vec2 coord)
//{
//	return fract(sin(dot(coord.xy, vec2(12.9898, 78.233))) * 43758.5453);
//}
//
//void main()
//{
//	vec2 coord = f_in.uv.xy;
//	vec3 color = vec3(0.0);
//	float grain = 0.0;
//
//	grain = random2d(vec2(sin(coord)) * u_time);
//
//	color = vec3(grain);
//
//	f_color = vec4(color, 1.0);
//}