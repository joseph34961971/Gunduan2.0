// Vertex shader for cartoon-style shading
// Author: Philip Rideout
// Copyright (c) 2005-2006 3Dlabs Inc. Ltd.
// See 3Dlabs-License.txt for license information
#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

uniform vec3 vLightPosition;

uniform MatVP{
	mat4 View;
	mat4 Projection;
};
uniform mat4 u_model;

// Color to fragment program
out vec3 vVaryingNormal;
out vec3 vVaryingLightDir;

void main(void) 
{ 
	mat4 MVP = Projection * View * u_model;
	mat4 MV = View * u_model;
	
    // Get surface normal in eye coordinates
	mat3 normalMatrix = mat3(MV);//normal matrix is MV matrix's 3*3 excluding 'w' 
    vVaryingNormal = normalMatrix * vertexNormal;

    // Get vertex position in eye coordinates
    vec4 vPosition4 = MV * vec4(vPosition, 1);
    vec3 vPosition3 = vPosition4.xyz / vPosition4.w;

    // Get vector to light source
    vVaryingLightDir = normalize(vLightPosition - vPosition3);

    // Don't forget to transform the geometry!
    gl_Position = MVP * vec4(vPosition, 1);
}