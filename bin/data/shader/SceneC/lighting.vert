#version 440

in vec4 position;
in vec2 texcoord;

out vec2 vTexCoord;

void main (){
    gl_Position = position;
    vTexCoord = texcoord;
}