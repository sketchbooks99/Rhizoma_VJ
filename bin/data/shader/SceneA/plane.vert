#version 440
#pragma include "../util/noise2D.frag"

uniform mat4 modelViewProjectionMatrix;
uniform float time;

in vec4 position;
in vec2 texcoord;
in vec3 normal;
in vec4 color;

out vec4 vColor;

void main() {
    float center2out = length(texcoord - 0.5) * 30.0;
    float n = cnoise(texcoord * 3.0 + time);
    float sin_val = sin(center2out) * n * 30.0;
    vec3 newVertex = position.xyz + normal * sin_val;
    gl_Position = modelViewProjectionMatrix * vec4(newVertex, 1.0);
    vColor = color;
    gl_PointSize = 3.0;
}