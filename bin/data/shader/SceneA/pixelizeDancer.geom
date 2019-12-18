#version 440

uniform mat4 modelViewProjectionMatrix;
uniform vec3 camPos;

layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

in Vertex {
    in vec2 texcoord;
    in vec3 normal;
    in vec4 color;
} vertex[];

out vec4 vColor;
out vec2 vTexCoord;
out vec3 vNormal;

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
    vec3 a = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 b = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 normal = normalize(cross(b,a));

    
}