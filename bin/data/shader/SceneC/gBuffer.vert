#version 440

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 mvpMatrix;

in vec4 position;
in vec3 normal;
in vec2 texcoord;
in vec4 color;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoord;
out vec3 vColor;

void main() {
	gl_Position = mvpMatrix * position;
	vTexCoord = texcoord;
	vPosition = (view * model * position).xyz;
	vNormal = normal;
	vColor = color.xyz;
}
