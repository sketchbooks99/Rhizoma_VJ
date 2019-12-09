#version 410 core
precision mediump float;

uniform mat4 invMatrix;
uniform mat4 model;
uniform vec3 lightDir;

in vec2 vTexCoord;
in vec3 vNormal;
in vec4 vColor;

out vec4 fragColor;

void main() {
    // mat4 invMatrix = inverse(modelViewProjectionMatrix);
    vec3 invLight = normalize(invMatrix * vec4(lightDir, 0.0)).xyz;

    float diffuse = max(dot(normalize(invLight), vNormal), 0.0);
    float specular = pow(diffuse, 20.0);
    vec3 ambient = vColor.rgb * 0.7;
    vec3 destColor = vColor.rgb * (ambient + diffuse) + specular;

    // fragColor = vec4(destColor, 1.0);
    fragColor = vec4(vColor);
}