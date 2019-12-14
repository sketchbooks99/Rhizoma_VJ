#version 440

layout(rgba32f, binding=0) uniform image2D posTex;
layout(rgba32f, binding=1) uniform image2D velTex;
layout(rgba32f, binding=2) uniform image2D forceTex;

const int BLOCK_SIZE = 1024;

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
uniform float alignmentRadius;
uniform float cohesionRadius;
// Maximum speed and force
uniform float maxSpeed;
uniform float maxForce;
// Weights of rules
uniform float separateWeight;
uniform float alignmentWeight;
uniform float cohesionWeight;
// The area settings where boids can move
uniform vec3 center;
uniform vec3 wallSize;
uniform float avoidWallWeight;
// Position of attractor
uniform vec3 attractorPos;
uniform float attractWeight;

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

shared Boids boidData[BLOCK_SIZE];

layout(local_size_x = BLOCK_SIZE, local_size_y = 1, local_size_z = 1) in;
void main() {
    // ===== Force calculation =====
    uint id = gl_GlobalInvocationID.x;
    vec3 myPos = b[id].pos;
    vec3 myVel = b[id].vel;

    vec3 force = vec3(0.0);

    // For calculating separate
    vec3 sepPosSum = vec3(0.0);
    int sepCount = 0;
    // For calculating alignment
    vec3 aliVelSum = vec3(0.0);
    int aliCount = 0;
    // For calculating cohesion
    vec3 cohPosSum = vec3(0.0);
    int cohCount = 0;

    uint gi = gl_LocalInvocationIndex.x;
    for(uint i = 0; i < uint(maxNum); i += BLOCK_SIZE) {
        boidData[gi] = b[i + gi];

        groupMemoryBarrier();

        for(int j = 0; j < BLOCK_SIZE; j++) {
            // Position & Velocity of other boid
            vec3 otherPos = boidData[j].pos;
            vec3 otherVel = boidData[j].vel;

            // Distance of position between other boid
            vec3 diff = myPos - otherPos;
            float dist = sqrt(dot(diff, diff));
            
            // Separation
            if (id != (i + gi) && dist <= separateRadius) {
                vec3 repulse = normalize(myPos - otherPos);
                repulse /= dist;
                sepPosSum += repulse;
                sepCount++;
            }

            // Alignment
            if (id != (i + gi) && dist <= alignmentRadius) {
                aliVelSum += otherVel;
                aliCount++;
            }

            // Cohesion
            // if (dist > 0.0) => if (id != i + gi)
            if (id != (i + gi) && dist <= cohesionRadius) {
                cohPosSum += otherPos;
                cohCount++;
            }
        }
        groupMemoryBarrier();
    }

    // Force (separation)
    vec3 sepSteer = vec3(0.0);
    if(sepCount > 0) {
        sepSteer = sepPosSum / float(sepCount);
        sepSteer = normalize(sepSteer) * maxSpeed;
        sepSteer = sepSteer - myVel;
        sepSteer = limit(sepSteer, maxForce);
    }

    // Force (alignment)
    vec3 aliSteer = vec3(0.0);
    if (aliCount > 0) {
        aliSteer = aliVelSum / float(aliCount);
        aliSteer = normalize(aliSteer) * maxSpeed;
        aliSteer = aliSteer - myVel;
        aliSteer = limit(aliSteer, maxForce);
    }

    // Force (cohesion)
    vec3 cohSteer = vec3(0.0);
    if (cohCount > 0) {
        cohPosSum = cohPosSum / float(cohCount);
        cohSteer = cohPosSum - myPos;
        cohSteer = normalize(cohSteer) * maxSpeed;
        cohSteer = cohSteer - myVel;
        cohSteer = limit(cohSteer, maxForce);
    }
    force += aliSteer * alignmentWeight;
    force += cohSteer * cohesionWeight;
    force += sepSteer * separateWeight;

    vec3 dir = attractorPos - myPos;
    force += normalize(dir) * attractWeight;

    force += avoidWall(myPos) * avoidWallWeight;

    imageStore(forceTex, ivec2(gl_GlobalInvocationID.xy), vec4(force, 1.0));
    f[id] = force;
}