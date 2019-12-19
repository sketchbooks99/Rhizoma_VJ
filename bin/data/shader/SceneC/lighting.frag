#version 440

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform vec3 camPos;
uniform vec3 lightPos;
uniform bool isShade;

in vec2 vTexCoord;

out vec4 fragColor;

void main() {
    vec3 position = texture(gPosition, vTexCoord).rgb;
    vec3 normal = texture(gNormal, vTexCoord).rgb;
    vec3 color = texture(gColor, vTexCoord).rgb;
    
    vec3 ambient = color * 0.2;

    vec3 lightDir = normalize(lightPos - position);
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * vec3(0.8);

    vec3 viewDir = normalize(camPos - position);
	vec3 halfDir = normalize(lightDir + viewDir);
	vec3 specular = vec3(pow(max(dot(normal, halfDir), 0.0), 16.0));

    fragColor = isShade ? vec4(ambient + diffuse + specular, 1.0) : vec4(color, 1.0);
    // fragColor = vec4(normal, 1.0);

}