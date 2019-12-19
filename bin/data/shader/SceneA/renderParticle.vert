#version 440

uniform mat4 modelViewProjectionMatrix;

in vec4 position;

struct Particle {
    vec3 pos;
    vec3 vel;
};

layout(std430, binding=1) buffer particles {
    Particle p[];
};

void main (){
    Particle par = p[gl_VertexID];
    gl_Position = modelViewProjectionMatrix * vec4(par.pos, 1.0);
}