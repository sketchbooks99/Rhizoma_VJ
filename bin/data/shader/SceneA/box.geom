#version 440

#pragma include "../util/util.frag"

layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

uniform mat4 modelViewProjectionMatrix;
uniform float time;

in Vertex {
    vec2 texcoord;
} vertex[];

out vec4 vColor;

// rule:
// v0 -> v1 -> v2の順で結ぶ
void createTriangle(vec3 v0, vec3 v1, vec3 v2) {
    vec3 faceNormal = vec3(0.0);
    vec3 a = v1 - v0;
    vec3 b = v2 - v0;
    faceNormal = normalize(cross(a, b));
    
    gl_Position = modelViewProjectionMatrix * vec4(v0, 1.0);
    vNormal = faceNormal;
    // vColor = vertex[0].color;
    vColor = vec4(1.0);
    EmitVertex();
    gl_Position = modelViewProjectionMatrix * vec4(v1, 1.0);
    vNormal = faceNormal;
    vColor = vec4(1.0);
    EmitVertex();
    gl_Position = modelViewProjectionMatrix * vec4(v2, 1.0);
    vNormal = faceNormal;
    vColor = vec4(1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {
    vec2 uv = vertex[0].texcoord;
    float n = cnoise(uv * 10.0 + time);
    for(int i=0; i<gl_in.length(); i++) {
        
    }
}