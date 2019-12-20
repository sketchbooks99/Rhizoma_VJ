#version 440

#pragma include "3d.glsl"

uniform float time;
uniform vec3 inColor;
uniform vec3 outColor;
uniform vec3 spColor;

in vec4 position;
in vec4 color;
in vec2 texcoord;
in vec3 normal;

out Vertex {
    vec4 color;
    vec2 texcoord;
} vertex;

void main() {
    float noiseValue = cnoise(position.xyz * 0.01 + vec3(0.0,0.0,time));
    float offset = noiseValue > 0.3 ? 0.4 : noiseValue < -0.3 ? -0.2 : 0.0;
    gl_Position = vec4(position.xyz + normal * offset * 150.0, 1.0);
    if(noiseValue > 0.3) vertex.color = vec4(outColor, .8);
    else {
        if(noiseValue < -0.3) vertex.color = vec4(inColor, 0.8);
        else vertex.color = vec4(spColor, 0.8);
    }
    vertex.texcoord = texcoord;
}