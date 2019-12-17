#version 440
precision mediump float;

const int BLOCK_SIZE = 1024;

layout(rgba32f, binding=0) uniform image2D posTex;

layout(std430, binding=0) buffer pos {
    vec3 p[];
};

layout(local_size_x = BLOCK_SIZE, local_size_y = 1, local_size_z = 1) in;
void main() {
    uint id = gl_GlobalInvocationID.x;
    imageStore(posTex, ivec2(gl_GlobalInvocationID.xy), vec4(p[id], 1.0));
}