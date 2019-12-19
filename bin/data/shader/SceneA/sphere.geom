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
uniform float volume;
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

    vec2 scaledUV = vertex[0].texcoord * 5.0;
    float n = cnoise(vec2(scaledUV.x * sin(time * 0.533), scaledUV.y * cos(time * 0.789))) + volume * 3.0;
    float height = n * 10.0;

    // for(int i = 0; i < gl_in.length(); i++) {
    //     vec3 newVertex = gl_in[i].gl_Position.xyz * 0.7 + center * 0.3 + normal * ;
    //     gl_Position = modelViewProjectionMatrix * vec4(newVertex, 1.0);
    //     vColor = vec4(1.0);
    //     vNormal = normal;
    //     EmitVertex();
    // }
    // EndPrimitive();
    vec3 v0, v1, v2, v3, v4, v5;
    // vec4 c = vec4(0.3, 0.2, 0.4, 0.5);
    vec4 c = vec4(vec3(0.2), 1.0);
    if (n < 0.5) {
        v0 = gl_in[0].gl_Position.xyz * n + center * (1.0 - n);
        v1 = gl_in[1].gl_Position.xyz * n + center * (1.0 - n);
        v2 = gl_in[2].gl_Position.xyz * n + center * (1.0 - n);
        createTriangle(v0, v1, v2, c);
    } else {
        vec3 v0 = gl_in[0].gl_Position.xyz * 0.9 + center * 0.1;
        vec3 v1 = gl_in[1].gl_Position.xyz * 0.9 + center * 0.1;
        vec3 v2 = gl_in[2].gl_Position.xyz * 0.9 + center * 0.1;
        vec3 v3 = gl_in[0].gl_Position.xyz * (1.0 - n) + center * n + normal * height;
        vec3 v4 = gl_in[1].gl_Position.xyz * (1.0 - n) + center * n + normal * height;
        vec3 v5 = gl_in[2].gl_Position.xyz * (1.0 - n) + center * n + normal * height;
        // bottom
        createTriangle(v0, v1, v2, c);
        // side
        // c = vec4(0.8, 0.3, 0.2, 1.0);
        c = vec4(1.0, 0.0, 0.0, 1.0);
        createTriangle(v3, v0, v4, c);
        createTriangle(v4, v0, v1, c);
        createTriangle(v5, v4, v1, c);
        createTriangle(v5, v1, v2, c);
        createTriangle(v5, v2, v3, c);
        createTriangle(v3, v2, v0, c);
        // top
        createTriangle(v3, v4, v5, c);
    }
}