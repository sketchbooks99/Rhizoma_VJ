#version 450

uniform mat4 modelViewProjectionMatrix;

layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;

out vec3 vNormal;
out vec4 vColor;

void main() {
    vec3 a = (gl_in[0].gl_Position - gl_in[1].gl_Position).xyz;
    vec3 b = (gl_in[0].gl_Position - gl_in[2].gl_Position).xyz;
    vec3 normal = normalize(cross(b, a));
    vec3 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position).xyz / 3.0;

    for(int i = 0; i < 3; i++) {
        gl_Position = modelViewProjectionMatrix * gl_in[i].gl_Position;
        vNormal = normal;
        // vColor = vec4(0.7, 1.0, 1.0, 1.0);
        vColor = vec4(0.5, 0.0, 0.0, 1.0);
        EmitVertex();
    }
    EndPrimitive();

    for(int i = 0; i < 3; i++) {
        vec3 newVertex = gl_in[i].gl_Position.xyz * 0.8 + center * 0.2 + normal * 0.01;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex, 1.0);
        vNormal = normal;
        vColor = vec4(vec3(0.1), 1.0);
        EmitVertex();
    }
    EndPrimitive();
}