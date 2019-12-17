#version 440

in vec4 vColor;
in vec3 vNormal;
in vec2 vTexCoord;

out vec4 fragColor;

uniform vec3 lightDir = vec3(-0.577, 0.577, -0.577);
uniform mat4 invMatrix;

void main() {
    // vec3 invLight = normalize(invMatrix * vec4(lightDir, 0.0)).xyz;
    vec3 invLight = lightDir;

    float diffuse = max(dot(invLight, vNormal), 0.0);
    float specular = pow(diffuse, 20.0);
    vec3 ambient = vColor.rgb * 0.3;
    vec3 destColor = ambient + diffuse + specular * 0.5;

    fragColor = vec4(destColor, 1.0);
}