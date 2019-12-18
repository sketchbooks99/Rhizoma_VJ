#version 440

in vec4 vColor;
in vec3 vNormal;

out vec4 fragColor;

uniform vec3 lightDir = vec3(0.577);
uniform mat4 invMatrix;
uniform float planeHeight;

void main() {
    fragColor = vec4(1.0);
}