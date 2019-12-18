#version 440

uniform mat4 modelViewProjectionMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

in Vertex {
    vec4 color;
    vec2 texcoord;
} vertex[];

out vec3 vNormal;
out vec4 vColor;

void main() {
    vec3 a = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 b = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 normal = normalize(cross(b, a));

    for(int i = 0; i < gl_in.length(); i++) {
        gl_Position = modelViewProjectionMatrix * gl_in[i].gl_Position;
        vNormal = normal;
        vColor = vertex[0].color;
        EmitVertex();
    }
    EndPrimitive();
}