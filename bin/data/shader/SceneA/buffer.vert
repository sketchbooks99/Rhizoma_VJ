#version 440

uniform mat4 mvpMatrix;
uniform int index;

in vec4 position;

layout(std430, binding=1) buffer pos {
    vec3 p[];
};

void main() {
    gl_Position = mvpMatrix * position;
    p[gl_VertexID + index] = position.xyz;
}