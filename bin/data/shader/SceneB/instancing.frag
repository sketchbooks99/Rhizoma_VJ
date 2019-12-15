#version 450
precision mediump float;

in vec4 vColor;
in vec3 vNormal;
out vec4 fragColor;

uniform mat4 invMatrix;

uniform vec3 lightDir = vec3(0.0, 1.0, 0.0);

void main() {
    // fragColor = vColor;
    vec3 invLight = normalize(invMatrix * vec4(lightDir, 0.0)).xyz;

    float diffuse = max(dot(invLight, vNormal), 0.0);
    float specular = pow(diffuse, 20.0);
    vec3 ambient = vColor.rgb * 0.3;
    vec3 destColor = vColor.rgb + specular * 0.5;

    // fragColor = vColor;
    fragColor = vec4(destColor, 1.0);
}