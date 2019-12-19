#version 440

in vec4 vColor;
in vec3 vNormal;

out vec4 fragColor;

uniform vec3 lightDir = vec3(0.577);
uniform mat4 invMatrix;
uniform float planeHeight;

void main() {
    // if (vColor.a < 0.1) {
    //     fragColor = vec4(vec3(0.0), 1.0);
    //     return;
    // }
    // // vec3 invLight = normalize(invMatrix * vec4(lightDir, 0.0)).xyz;

    // float diffuse = max(dot(lightDir, vNormal), 0.0);
    // float specular = pow(diffuse, 20.0);
    // vec3 ambient = vColor.rgb * 0.3;
    // vec3 destColor = ambient + diffuse + specular * 0.5;

    // if (planeHeight <= 0.0) destColor = vColor.rgb;

    // fragColor = vec4(destColor, vColor.a);
    fragColor = vColor;
}