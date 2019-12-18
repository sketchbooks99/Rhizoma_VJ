#version 440

in vec3 vNormal;
in vec4 vColor;
in vec4 vPosition;

const int LIGHT_NUM = 3;

uniform mat4 model;
uniform vec3 lightPosition[LIGHT_NUM];
uniform vec3 viewPos;
uniform float lightAttenuation;
uniform float disCoef;

out vec4 fragColor;

void main() {
    vec3 lighting = vColor.rgb * 0.3;
    for(int i=0; i<LIGHT_NUM; i++) {
        vec3 lightPos = (vec4(lightPosition[i], 1.0)).xyz;
        vec3 lightDir = (lightPos - vPosition.xyz);
        float lightLen = length(lightDir) * disCoef;
        float attenuation = 1.0 / (lightAttenuation * lightLen * lightLen);
        lightDir = normalize(lightDir);
        float diffuse = max(0.0, dot(vNormal, lightDir));
        vec3 diffuseColor = max(0.0, dot(vNormal, lightDir)) * vec3(1.0);
        vec3 specularColor = vec3(pow(diffuse, 20.0)) * vec3(1.0);
        lighting += (diffuseColor + specularColor) * attenuation;
    }
    fragColor = vec4(lighting, 1.0);
}