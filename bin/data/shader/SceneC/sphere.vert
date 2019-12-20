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
    gl_Position = position;
    vertex.texcoord = texcoord;
    vertex.color = color;
}