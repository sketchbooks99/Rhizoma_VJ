#version 440

in vec3 vColor;
in vec3 vNormal;
in vec3 vPosition;
in vec2 vTexCoord;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gColor;

uniform vec3 lightDir = vec3(0.577);

void main() {
    gPosition = vec4(vPosition, 1.0);
    gNormal = vec4(vNormal, 1.0);
    gColor = vec4(vColor, 1.0);
    // fragColor = vec4(1.0);
    // fragColor = vec4(destColor, 1.0);
    // fragColor = vec4(vColor);
}