#version 440

#pragma include "../util/noise2D.frag"

layout(triangles) in;
layout(triangle_strip, max_vertices = 36) out;

// uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform float intensity;
uniform bool isColored;
uniform float planeHeight;
uniform float time;
uniform int threshold;

in Vertex {
    vec2 texcoord;
    vec4 color;
} vertex[];

out vec4 vColor;
out vec3 vNormal;

vec3 hsb2rgb( vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                    6.0)-3.0)-1.0,
                    0.0,
                    1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

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
    vec3 a = (gl_in[1].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 b = (gl_in[2].gl_Position - gl_in[0].gl_Position).xyz;
    vec3 normal = normalize(cross(b,a));
    vec3 center = (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position).xyz / 3.0;

    vec2 uv = vertex[0].texcoord;
    for(int i = 0; i < gl_in.length(); i++) {
        vec3 newVertex = gl_in[i].gl_Position.xyz * 0.7 + center * 0.3;
        gl_Position = modelViewProjectionMatrix * vec4(newVertex, 1.0);
        vColor = vec4(1.0);
        vNormal = normal;
        EmitVertex();
    }
    EndPrimitive();
}