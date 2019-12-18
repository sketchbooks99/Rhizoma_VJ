#version 440

#pragma include "../util/noise2D.frag"

uniform float time;

in vec4 position;
in vec2 texcoord;
in vec3 normal;
in vec4 color;

out Vertex {
    vec2 texcoord;
    vec4 color;
} vertex;

void main() {
    vec2 scaledUV = texcoord * 5.0;
    float n = cnoise(vec2(scaledUV.x * sin(time * 0.533), scaledUV.y * cos(time * 0.789)));
    vec3 newVertex = position.xyz + normal * n * 10.0;
    gl_Position = vec4(newVertex, 1.0);
    vertex.texcoord = texcoord;
    vertex.color = color;
}