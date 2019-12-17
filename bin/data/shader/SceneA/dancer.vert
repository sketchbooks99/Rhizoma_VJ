#version 440

in vec4 position;
in vec2 texcoord;
in vec3 normal;
in vec4 color;

out Vertex {
    vec2 texcoord;
    vec3 normal;
    vec4 color;
} vertex;

void main() {
    gl_Position = position;
    vertex.texcoord = texcoord;
    vertex.normal = normal;
    vertex.color = color;
}