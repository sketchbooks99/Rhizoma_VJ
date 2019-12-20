#version 450
precision mediump float;

in vec3 vColor;
in vec3 debug;

out vec4 fragColor;

void main() {
//    fragColor = vec4(1.0);
	fragColor = vec4(vColor, 1.0);
}