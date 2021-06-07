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

// lighting color
vec4    ambientColor = vec4(0.1,0.1,0.1,1);
vec4    diffuseColor = vec4(0.8,0.8,0.8,1);   
vec4    specularColor = vec4(1,1,1,1);

in vec3 vVaryingNormal;
in vec3 vVaryingLightDir;
in vec2 UV;
float Shininess = 128.0; // for material specular
uniform bool dissolveGray;
uniform sampler2D dissolveTex;
uniform float dissolveThreshold;
uniform float alpha;
uniform bool useLighting;

in vec4 FragPosLightSpace;
uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    	// perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(vVaryingNormal);
    vec3 lightDir = normalize(vVaryingLightDir);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

vec3 gray(vec3 color)
{
    return vec3(color.r * 0.3 + color.g * 0.59 + color.b * 0.11);
}

void main(void)
{ 
    if (useLighting)
    {
		float shadow = ShadowCalculation(FragPosLightSpace);

        // Dot product gives us diffuse intensity
        float diff = max(0.0, dot(normalize(vVaryingNormal),
					    normalize(vVaryingLightDir)));

        // Multiply intensity by diffuse color, force alpha to 1.0
        vFragColor = (1.0 - shadow / 2.0) * diff * diffuseColor * vec4(Material.Kd,1);

        // Add in ambient light
        vFragColor += ambientColor;


        // Specular Light
        vec3 vReflection = normalize(reflect(-normalize(vVaryingLightDir),
								    normalize(vVaryingNormal)));//反射角
        float spec = max(0.0, dot(normalize(vVaryingNormal), vReflection));
        if(diff != 0) {
		    spec = pow(spec, Shininess);
		    vFragColor += (1.0 - shadow / 2.0) * specularColor * vec4(Material.Ka,1) * spec;
        }
    }
    else
    {
        vFragColor = vec4(Material.Kd,1);
    }

    if (dissolveGray)
    {
        if (texture(dissolveTex, UV).r > dissolveThreshold)
        {
            vec4 color = vec4(gray(vec3(vFragColor)), 1.0);
            vFragColor = color;
        }
    }

    vFragColor = vec4(vec3(vFragColor), alpha);
}