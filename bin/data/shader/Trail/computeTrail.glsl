#version 440

#pragma include "noise4D.frag"
uniform int trailLength;
uniform float time;
uniform float timestep;
uniform float speed;

const int BLOCK_SIZE = 1024;

struct Trail {
    vec4 pos;
    vec4 vel;
};

layout(std430, binding=0) buffer trail {
    Trail t[];
};

vec3 curlNoise(vec3 p, float id) {
    return vec3(
        snoise(vec4(0.15 * p, 7.225 + time * 0.1)),
        snoise(vec4(0.15 * p, 3.552 + time * 0.1)),
        snoise(vec4(0.15 * p, 1.259 + time * 0.1))
    );
}

layout(local_size_x = BLOCK_SIZE, local_size_y = 1, local_size_z = 1) in;
void main() {
    uint id = gl_GlobalInvocationID.x;
    vec3 myPos = t[id].pos.xyz;
    vec3 myVel = t[id].vel.xyz;
    if(id % trailLength == 0) {
        vel += 40.0 * curlNoise(pos, id) * 0.2;
        vel += -pos * length(pos) * 0.05;
        vel *= 0.9 + abs(sin(id * 9.0)) * 0.03;
        pos += vel * 0.01 * speed;
    } else {
        pos = t[id - 1].pos;
    }
}