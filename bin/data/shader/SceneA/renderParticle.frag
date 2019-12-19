#version 440
precision mediump float;

in vec4 vColor;
in vec3 vNormal;
in vec3 vPosition;

// layout(location = 0) out vec4 gPosition;
// layout(location = 1) out vec4 gNormal;
// layout(location = 2) out vec4 gColor;
out vec4 fragColor;

void main() {
	fragColor = vColor;
}
	 