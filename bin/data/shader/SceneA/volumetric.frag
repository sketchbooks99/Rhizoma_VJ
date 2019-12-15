#version 410 core
precision mediump float;

uniform sampler2D occludeTex;
uniform sampler2D colorTex;

uniform vec3 screenLightPos;
uniform float density;
uniform float weight;
uniform float decay;
uniform float exposure;

in vec2 vTexCoord;
in vec4 vColor;

out vec4 fragColor;

const float PI = 3.14159265359;
const int NUM_SAMPLES = 200;

void main() {
    vec2 texCoord = vTexCoord;
    // Calculate vector from pixel to light source in screen space
    vec2 deltaTexCoord = vTexCoord - screenLightPos.xy;
    // Divide by number of samples and scale by control factor
    deltaTexCoord *= 1.0 / float(NUM_SAMPLES) * density;
    // Store Initial sample
    vec3 color = texture(occludeTex, texCoord).xyz;
    float exposure = texture(occludeTex, texCoord).a;
    // Set up illumination decay factor
    float illuminationDecay = 0.5;
    // Evaluate summation
    for (int i = 0; i < NUM_SAMPLES; i++) {
        texCoord -= deltaTexCoord;
        vec3 s = texture(occludeTex, texCoord).xyz;
        s *= illuminationDecay * weight;
        color += s;
        illuminationDecay *= decay;
    }
    fragColor = texture(colorTex, vTexCoord) + vec4(color * exposure, 1.0);
    // fragColor = vec4(color * exposure, 1.0);
    // fragColor = texture(occludeTex, vTexCoord);
    // fragColor = texture(colorTex, vTexCoord) + texture(occludeTex, vTexCoord);
}