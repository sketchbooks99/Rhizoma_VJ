#version 450

#pragma include "2d.glsl"

#define PI 3.141529

struct Trail {
    vec4 pos;
    vec4 vel;
};

layout(std430, binding=0) buffer trail {
    Trail t[];
};

uniform sampler2DRect posTex;
uniform int trailLength;
uniform mat4 modelViewProjectionMatrix;
uniform vec3 camPos;
uniform int numTrail;
uniform float size;
uniform float trailWidth;
uniform float time;

layout(triangles) in;
layout(triangle_strip, max_vertices = 132) out;

in Vertex {
    vec3 pos;
    int id;
    vec2 texcoord;
} vertex[];

out vec3 vColor;
out vec3 debug;

float rand(vec2 p){
  return fract(sin(dot(p.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec3 rand3d(vec3 p) {
    return vec3(
        rand(p.xy + time * 0.1),
        rand(p.yz + time * 0.1),
        rand(p.zx + time * 0.1)
    );
}

void main() {
    float width = trailWidth * 0.5;

    for(int i=0; i<trailLength; i++) {
        vec3 dir = vec3(0.0);
        vec3 sideDir = vec3(0.0);
        vec3 pos = 
        vec3 pos = t[vertex[0].id + i]).xyz * size;
        if(i < trailLength - 1) {
            dir = pos - t[vertex[0].id + (i+1)].pos.xyz * size;// texture(posTex, vertex[0].texcoord + vec2(float(i+1), 0.0)).xyz * size;
            vec3 toCamDir = normalize(camPos - dir);
            sideDir = normalize(cross(toCamDir, dir));
        }
        vColor = (vertex[0].id % 5 == 0) ? vec3(0.5, 0.0, 0.0) : vec3(0.0);
		debug = vec3(1.0);
        gl_Position = modelViewProjectionMatrix * vec4(pos + sideDir * width, 1.0);
        EmitVertex();
//		vColor = (vertex[0].id % 5 == 0) ? vec4(0.0, 0.2, 0.3, 1.0) : vec4(vec3(0.0), 0.7);
        gl_Position = modelViewProjectionMatrix * vec4(pos - sideDir * width, 1.0);
        EmitVertex();   
    }

    EndPrimitive();
}
