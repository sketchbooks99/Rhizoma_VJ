#version 440

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gColor;

in vec2 vTexCoord;
in vec3 vPosition;
in vec3 vNormal;
in vec3 vColor;

void main() {
	gPosition = vec4(vPosition, 1.0);
	gNormal = vec4(vNormal, 1.0);
	gColor = vec4(vColor, 1.0);
}
