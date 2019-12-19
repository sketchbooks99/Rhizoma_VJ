#version 440

in vec4 position;
in vec3 normal;
in vec2 texcoord;

out Vertex {
    vec2 texcoord;
} vertex;

void main() {
    gl_Position = position;
    vertex.texcoord = texcoord;
}