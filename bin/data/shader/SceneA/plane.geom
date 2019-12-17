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

    vec2 uv = vertex[0].texcoord;
    float n = cnoise(uv * 100.0 + time);
    vec3 col = isColored ? hsb2rgb(vec3(n, 1.0, 1.0)) : vec3(1.0);
    for(int i = 0; i < gl_in.length(); i++) {
        gl_Position = modelViewProjectionMatrix * gl_in[i].gl_Position;
        // vColor = n > threshold ? vec4(1.0) : vec4(0.0);
        vColor = n > threshold ? vec4(col, 1.0) : vec4(0.0);
        vNormal = normal;
        // vColor = vec4(n);
        EmitVertex();
    }
    EndPrimitive();

    if(planeHeight > 0 && n > 0.5) {
        vec3 v0 = gl_in[0].gl_Position.xyz;
        vec3 v1 = gl_in[1].gl_Position.xyz;
        vec3 v2 = gl_in[2].gl_Position.xyz;
        vec3 v3 = gl_in[0].gl_Position.xyz + normal * planeHeight;
        vec3 v4 = gl_in[1].gl_Position.xyz + normal * planeHeight;
        vec3 v5 = gl_in[2].gl_Position.xyz + normal * planeHeight;

        // side face 1
        createTriangle(v0, v1, v3, vec4(col, 1.0));
        createTriangle(v3, v1, v4, vec4(col, 1.0));
        // side face 2
        createTriangle(v1, v2, v4, vec4(col, 1.0));
        createTriangle(v4, v2, v5, vec4(col, 1.0));
        // side face 3
        createTriangle(v5, v2, v3, vec4(col, 1.0));
        createTriangle(v2, v0, v3, vec4(col, 1.0));
        // top
        createTriangle(v3, v4, v5, vec4(col, 1.0));
    }
}