#version 440

#pragma include "../util/noise2D.frag"
#pragma include "../util/util.frag"

uniform float time;

in vec4 position;
in vec3 normal;
in vec2 texcoord;

out Vertex {
    vec4 color;
    vec2 texcoord;
} vertex;

void main() {
    float n = cnoise(texcoord * 3.0 + vec2(0.0, -time * 0.5));

    vec3 newVertex = position.xyz + normal * n * 150.0;
    gl_Position = vec4(newVertex, 1.0);
    vertex.color = vec4(0.6, 0.6, 0.2, 1.0);
    vertex.texcoord = texcoord;
}