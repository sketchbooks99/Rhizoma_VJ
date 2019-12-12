#version 400
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewmatrix;

in vec4 position;
in vec2 texcoord;
in vec4 color;

uniform sampler2DRect posTex;

out vec4 vColor;

void main() {
    vec2 size = textureSize(posTex);
    ivec2 uv = ivec2(
        mod(float(gl_InstanceID), size.x),
        floor(float(gl_InstanceID) / size.x)
    );
    vec4 p = texelFetch(posTex, uv);

    gl_Position = modelViewProjectionMatrix * vec4(p.xyz, 1.0);
    vColor = color;
}