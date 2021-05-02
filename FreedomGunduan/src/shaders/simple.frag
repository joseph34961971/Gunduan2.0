#version 430 core
out vec4 f_color;

in V_OUT
{
   vec3 position;
   vec3 normal;
   vec2 texture_coordinate;
   vec3 viewDir;
} f_in;

uniform sampler2D u_texture;

// lighting color
vec3    ambientColor = vec3(0.5,0.5,0.5);
vec3    diffuseColor = vec3(0.8,0.8,0.8);   
vec3    specularColor = vec3(1,1,1);
float   Shininess = 128.0; // for material specular
uniform vec3 vLightPosition;

vec3 CalcDirLight(vec3 normal, vec3 viewDir)
{
    vec3 color = texture(u_texture, f_in.texture_coordinate).rgb;
    normal = normalize(normal);
    // ambient
    vec3 ambient = ambientColor * color;
    // diffuse
    vec3 lightDir = normalize(-vLightPosition);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * diffuseColor;
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), Shininess);
    vec3 specular = spec * specularColor;              
    return (ambient + diffuse + specular) * color;
}

void main()
{   
    f_color = vec4(CalcDirLight(f_in.normal, f_in.viewDir), 1);
}