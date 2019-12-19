#version 440
precision mediump float;

const int BLOCK_SIZE = 1024;

#pragma include "../util/util.frag"
#pragma include "../util/noise4D.frag"

struct Lifetime {
    float age;
    float maxAge;
};

struct Particle {
    vec3 pos;
    vec3 vel;
};

layout(std430, binding=0) buffer spawn {
    vec3 sp[];
};

layout(std430, binding=1) buffer pos {
    Particle p[];
};

layout(std430, binding=2) buffer lifetimes {
    Lifetime l[];
};

uniform float time;
uniform float timestep;
uniform float scale;

layout(local_size_x = BLOCK_SIZE, local_size_y = 1, local_size_z = 1) in;
void main() {
    uint id = gl_GlobalInvocationID.x;
    uint sp_idx = id % 10;
    
    vec3 sp_pos = sp[sp_idx];
    Particle par = p[id];
    vec3 pos = par.pos;
    vec3 vel = par.vel;

    Lifetime life = l[id];
    float age = life.age;
    float maxAge = life.maxAge;
    age += 0.01;

    if (age >= maxAge) {
        age = 0.0;
		// maxAge = 1.0 + random(pos.xy) * 1.0;
        maxAge = 0.7;
        // maxAge = 1.0;
        float theta = 2.0 * PI * random(pos.yy);
        float phi = PI * random(pos.zz);
        float r = random(pos.xy);
		pos = vec3(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta)) * 10.0;
        // pos = vec3(random(pos.xx + time * 0.1), random(pos.yy + time * 0.3), random(pos.zz + time * 0.5)) * 100.0;
		vel = normalize(pos);
    }

    vel.x += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 0.1353 * time * timestep));
	vel.y += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 1.2814 * time * timestep));
	vel.z += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 2.5564 * time * timestep));

    pos += vel * 0.5;

    par.pos = pos;
    par.vel = vel;
    p[id] = par;

    life.age = age;
    life.maxAge = maxAge;
    l[id] = life;
}