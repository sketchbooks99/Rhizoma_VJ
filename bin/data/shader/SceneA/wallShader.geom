#version 440

uniform mat4 modelViewProjectionMatrix;
layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

in float volume[];

out vec4 vColor;
out vec3 vNormal;

// rule:
// v0 -> v1 -> v2の順で結ぶ
void createTriangle(vec3 v0, vec3 v1, vec3 v2, vec4 color) {
    vec3 faceNormal = vec3(0.0);
    vec3 a = v1 - v0;
    vec3 b = v2 - v0;
    faceNormal = normalize(cross(a, b));
    
    gl_Position = modelViewProjectionMatrix * vec4(v0, 1.0);
    vNormal = faceNormal;
    // vColor = vertex[0].color;
    vColor = color;
    EmitVertex();
    gl_Position = modelViewProjectionMatrix * vec4(v1, 1.0);
    vNormal = faceNormal;
    vColor = color;
    EmitVertex();
    gl_Position = modelViewProjectionMatrix * vec4(v2, 1.0);
    vNormal = faceNormal;
    vColor = color;
    EmitVertex();
    EndPrimitive();
}

void main() {
    // vec3 v0 = gl_in[0].gl_Position.xyz + vec3(-1.0, volume[0] * 10.0, 0.0);
    // vec3 v1 = gl_in[0].gl_Position.xyz + vec3(1.0, volume[0] * 10.0, 0.0);
    // vec3 v2 = gl_in[0].gl_Position.xyz + vec3(-1.0, -volume[0] * 10.0, 0.0);
    // vec3 v3 = gl_in[0].gl_Position.xyz + vec3(1.0, -volume[0] * 10.0, 0.0);
    vec3 v0 = gl_in[0].gl_Position.xyz + vec3(-1.0, 10.0, 0.0);
    vec3 v1 = gl_in[0].gl_Position.xyz + vec3(1.0, 10.0, 0.0);
    vec3 v2 = gl_in[0].gl_Position.xyz + vec3(-1.0, -10.0, 0.0);
    vec3 v3 = gl_in[0].gl_Position.xyz + vec3(1.0, -10.0, 0.0);

    createTriangle(v0, v2, v3, vec4(1.0));
    createTriangle(v0, v3, v1, vec4(1.0));
}