#version 440

uniform mat4 modelViewProjectionMatrix;
uniform float time;
uniform bool isColored;

struct Particle {
    vec3 pos;
    vec3 vel;
};

struct Lifetime {
    float age;
    float maxAge;
};
layout(std430, binding=1) buffer particles {
    Particle p[];
};
layout(std430, binding=2) buffer lifetimes {
    Lifetime l[];
};

out vec4 vColor;

void main() {
    Particle par = p[gl_VertexID];
    Lifetime life = l[gl_VertexID];
    gl_Position = modelViewProjectionMatrix * vec4(par.pos, 1.0);
    // vColor = vec4(0.4, 0.8, 0.8, 0.6);
    // vColor = vec4(1.0);
    vColor = isColored ? vec4(mod(time * 0.3, 1.0), 0.5, life.age, 1.5 - life.age) : vec4(1.0);
    gl_PointSize = 3.0f;
}