#version 440

uniform mat4 mvpMatrix;
uniform mat4 modelViewProjectionMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices = 6) out;

in Vertex {
    vec2 texcoord;
    vec3 normal;
    vec4 color;
} vertex[];

out vec4 vColor;
out vec3 vNormal;
out vec2 vTexCoord;

void main() {
    vec3 a = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 b = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 normal = normalize(cross(b, a));
    vec3 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position).xyz;

    for(int i = 0; i < gl_in.length(); i++) {
        vNormal = vertex[i].normal;
        vTexCoord = vertex[i].texcoord;
        vColor = vec4(1.0);
        gl_Position = mvpMatrix * gl_in[i].gl_Position;
        // gl_Position = modelViewProjectionMatrix * gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}