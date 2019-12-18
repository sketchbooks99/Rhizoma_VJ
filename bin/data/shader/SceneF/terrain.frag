#version 440
precision mediump float;

uniform vec3 lightDir = vec3(0.577, -0.577, -0.577);

in vec4 vColor;
in vec3 vNormal;

out vec4 fragColor;

void main() {

    float diffuse = max(dot(lightDir, vNormal), 0.0);
    float specular = pow(diffuse, 20.0);
    vec3 ambient = vColor.rgb * 0.3;
    vec3 destColor = vColor.rgb * (ambient + diffuse) + specular * 0.5;

    // fragColor = vColor;
    fragColor = vec4(destColor, 1.0);
}