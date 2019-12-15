#version 440

const int BLOCK_SIZE = 1024;

#pragma include "../util/util.frag"
#pragma include "../util/noise4D.frag"

// layout(rgb32f, binding=0) uniform image2D posTex;
// layout(rgb32f, binding=1) uniform image2D velTex;
// layout(rgb32f, binding=2) uniform image2D forceTex;

struct Boids {
    vec3 pos;
    vec3 vel;
};

struct Lifetime {
    float age;
    float maxAge;
};

layout(std430, binding=1) buffer boids {
    Boids b[];
};

layout(std430, binding=2) buffer lifetime {
    Lifetime l[];
};

uniform float time;
uniform float timestep;
uniform float scale;

layout(local_size_x = BLOCK_SIZE, local_size_y = 1, local_size_z = 1) in;
void main() {
    uint id = gl_GlobalInvocationID.x;

    // Data of boids
    Boids boid = b[id];
    vec3 pos = boid.pos;
    vec3 vel = boid.vel;

    // Life time
    Lifetime life = l[id];
    float age = life.age;
    float maxAge = life.maxAge;
    age += 0.01;
    
    if (age >= maxAge) {
        age = 0.0;
		maxAge = 1.0;
        float theta = 2.0 * PI * random(pos.yy);
        float phi = PI * random(pos.zz);
        float r = random(pos.xy);
		pos = vec3(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta)) * 20.0;
		vel = normalize(pos);
    }

    vel.x += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 0.1353 * time * timestep));
	vel.y += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 1.2814 * time * timestep));
	vel.z += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 2.5564 * time * timestep));

	pos += vel * 0.5;

    boid.pos = pos;
    boid.vel = vel;
    b[id] = boid;

    life.age = age;
    life.maxAge = maxAge;
    l[id] = life;
}
