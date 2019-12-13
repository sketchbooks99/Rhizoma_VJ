#version 440

const int BLOCK_SIZE = 1024;

layout(rgba32f, binding=0) uniform image2D posTex;
layout(rgba32f, binding=1) uniform image2D velTex;
layout(rgba32f, binding=2) uniform image2D forceTex;

struct Boids {
    vec3 pos;
    vec3 vel;
};

layout(std430, binding=0) buffer force {
    vec3 f[];
};

layout(std430, binding=1) buffer boids {
    Boids b[];
};

uniform int maxNum;
uniform float deltaTime;
// distance between neibourhood
uniform float separateRadius;
uniform float alimentRadius;
uniform float cohesionRadius;
// Maximum speed and force
uniform float maxSpeed;
uniform float maxForce;
// Weights of rules
uniform float separateWeight;
uniform float alimentWeight;
uniform float cohesionWeight;
// The area settings where boids can move
uniform vec3 center;
uniform vec3 wallSize;
uniform float avoidWallWeight;

// limiting vector size
vec3 limit(vec3 vec, float max) {
    float len = sqrt(dot(vec, vec)); // size
    return (len > max && len > 0) ? vec.xyz * (max / len) : vec.xyz;
}

// Avoiding from wall
vec3 avoidWall(vec3 pos) {
    vec3 wc = center.xyz;
    vec3 ws = wallSize.xyz;
    vec3 acc = vec3(0.0);
    // x
    acc.x = (pos.x < wc.x - ws.x * 0.5) ? acc.x + 1.0 : acc.x;
    acc.x = (pos.x > wc.x + ws.x * 0.5) ? acc.x - 1.0 : acc.x;
    // y
    acc.y = (pos.y < wc.y - ws.y * 0.5) ? acc.y + 1.0 : acc.y;
    acc.y = (pos.y > wc.y + ws.y * 0.5) ? acc.y - 1.0 : acc.y;
    // z
    acc.z = (pos.z < wc.z - ws.z * 0.5) ? acc.z + 1.0 : acc.z;
    acc.z = (pos.z > wc.z + ws.z * 0.5) ? acc.z - 1.0 : acc.z;

    return acc;
}

layout(local_size_x = BLOCK_SIZE, local_size_y = 1, local_size_z = 1) in;
void main() {
    uint id = gl_GlobalInvocationID.x;
    Boids boid = b[id];
    vec3 force = f[id];

    force += avoidWall(boid.pos) * avoidWallWeight;

    boid.vel += force * deltaTime;
    boid.vel = limit(boid.vel, maxSpeed);
    boid.pos += boid.vel * deltaTime;

    imageStore(posTex, ivec2(gl_GlobalInvocationID.xy), vec4(boid.pos, 1.0));
    imageStore(velTex, ivec2(gl_GlobalInvocationID.xy), vec4(boid.vel, 1.0));
    b[id] = boid;
}
