#version 410 core
precision mediump float;

// Include utility
#pragma include "../util/util.frag"
#pragma include "../util/noise4D.frag"

uniform sampler2DRect posAndAgeTex;
uniform sampler2DRect velAndMaxTex;
uniform float time;
uniform float timestep;
uniform float scale;

in vec2 vTexCoord;

layout(location = 0) out vec4 outPosData;
layout(location = 1) out vec4 velPosData;
//out vec4 outPosData;
//out vec4 velPosData;


void main() {
	// Get position and lifetime of particle from texture
	vec4 posAndAge = texture(posAndAgeTex, vTexCoord);
	// Get velocity and maximum lifetime from texture
	vec4 velAndMax = texture(velAndMaxTex, vTexCoord);

	vec3 pos = posAndAge.xyz;
	vec3 vel = velAndMax.xyz;

	float age = posAndAge.w;
	float maxAge = velAndMax.w;

	age += 0.1;

	if(age >= maxAge) {
		age = 0.0;
		maxAge = 1.0;
		vec3 startPos = vec3(0.0);
		pos.xyz = 2.0 * vec3(random(pos.xx), random(pos.yy), random(pos.zz)) - 1.0;
		vel.xyz = normalize(pos);
	}

	// Update velocity by Curl noise
	vel.x += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 0.1353 * time * timestep));
	vel.y += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 1.2814 * time * timestep));
	vel.z += snoise(vec4(pos.x * scale, pos.y * scale, pos.z * scale, 2.5564 * time * timestep));

	pos += vel * 0.5;

	outPosData = vec4(pos, age);
	velPosData = vec4(vel, maxAge);
//	outPosData = vec4(vTexCoord, 1.0, 1.0);
//	velPosData = vec4(vTexCoord, 1.0, 1.0);
}

