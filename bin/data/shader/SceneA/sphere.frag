#version 440

in vec4 vColor;
in vec3 vNormal;

out vec4 fragColor;

uniform vec3 lightDir = vec3(0.577);

void main() {
    float diffuse = max(dot(lightDir, vNormal), 0.0);
    float specular = pow(diffuse, 20.0);
    vec3 ambient = vColor.rgb * 0.3;
    vec3 destColor = ambient + diffuse + specular * 0.5;

    // fragColor = vec4(1.0);
    // fragColor = vec4(destColor, 1.0);
    fragColor = vec4(vColor);
}