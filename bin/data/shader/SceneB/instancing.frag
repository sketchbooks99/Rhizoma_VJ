#version 450

in vec4 vColor;
in vec3 vNormal;
out vec4 fragColor;

uniform mat4 invMatrix;

uniform vec3 lightDir = vec3(0.0, 1.0, 0.0);

void main() {
    // fragColor = vColor;

    fragColor = vColor;
}