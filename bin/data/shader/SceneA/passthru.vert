#version 410 core

uniform mat4 modelViewProjectionMatrix;

in vec2 texcoord;
in vec4 position;

out vec2 vTexCoord;

void main() {
	gl_Position = modelViewProjectionMatrix * position;
	vTexCoord = texcoord;
}