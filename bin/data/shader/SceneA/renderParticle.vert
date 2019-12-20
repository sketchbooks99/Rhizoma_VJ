#version 440

uniform mat4 modelViewProjectionMatrix;
uniform mat4 mvpMatrix;
uniform float time;
uniform bool isColored;
uniform vec3 pointColor;

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
    if(pointColor.r < 0.0)
        vColor = isColored ? vec4(mod(time * 0.3, 1.0), 0.5, life.age, 1.5 - life.age) : vec4(1.0);
    else vColor = vec4(pointColor, 0.7);
    gl_PointSize = 3.0f;
}