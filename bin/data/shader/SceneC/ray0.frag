#version 440
precision mediump float;

uniform float time;
uniform vec2 resolution;
uniform mat4 view;
uniform int sceneMode;
uniform float volume;
uniform bool isKicked;
uniform mat4 projection;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec3 camPos;
uniform vec3 camUp;
uniform float fov;
uniform float farClip;
uniform float nearClip;

in vec2 vTexCoord;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gColor;

const float PI = 3.14159265;
const float TWO_PI = PI * 2.0;
const float angle = 60.0;
const float DEG2RAD = (PI/180.0);

vec3 cPos = vec3(0.0, 0.0, 2.0);
const float sphereSize = 1.0;
const vec3 lightDir = vec3(-0.577, 0.577, 0.577);

struct Object {
	float dist;
	vec3 col;
};

vec3 rotate(vec3 p, float angle, vec3 axis){
    vec3 a = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float r = 1.0 - c;
    mat3 m = mat3(
        a.x * a.x * r + c,
        a.y * a.x * r + a.z * s,
        a.z * a.x * r - a.y * s,
        a.x * a.y * r - a.z * s,
        a.y * a.y * r + c,
        a.z * a.y * r + a.x * s,
        a.x * a.z * r + a.y * s,
        a.y * a.z * r - a.x * s,
        a.z * a.z * r + c
    );
    return m * p;
}

mat2 rotate(float a) {
    float s = sin(a), c = cos(a);
    return mat2(c, s, -s, -c);
}

float smoothMin(float d1, float d2, float k) {
	float h = exp(-k  * d1) + exp(-k * d2);
	return -log(h) / k;
}

float box(vec3 p, float size) {
	vec3 q = abs(p);
	return length(max(q - vec3(size), 0.0));
}

vec3 trans(vec3 p) {
	return mod(p, 2.0) - 1.0;
}

float sphere(vec3 p, float size) {
	return length(p) - size;
}

float sdFloor(vec3 p, vec3 n) {
	return dot(p, n) + 1.0;	
}

float scene1(vec3 p) {
	float fl = sdFloor(p, vec3(0.0, 1.0, 0.0));
	float fl2 = sdFloor(p, vec3(0.0, -1.0, 0.0));
	float sp = sphere(p, 0.7 + volume);
	for(float i = 0.; i < 4.; i++) {
		float x = sin(i * TWO_PI / 4.) * 2.0;
		float z = cos(i * TWO_PI / 4.) * 2.0;
		if(sceneMode == 1) {
			x += volume;
			z += volume;
		}
		float sp1 = sphere(p + vec3(x, 0., z), 0.7);
		sp = smoothMin(sp, sp1, 5.0);
	}
	return smoothMin(sp, min(fl, fl2), 5.0);
}

float scene2(vec3 p) {
	float iter = 4.;
	float time0 = time;
	if(sceneMode == 3) {
		iter = 8.;
		time0 *= 0.5;
	}
	float radius = sceneMode == 2 ? 2. : 3.;
	float size = sceneMode == 2 ? 1.0 : 2.0;
	vec3 trans = vec3(sin(time0), cos(time0 * 1.589), cos(time0 * 1.23)) * 2.0;
	float sp = sphere(p + trans, size + volume);
	for(float i = 0.; i < iter; i++) {
		float x = sin(time * i * 1.46) * radius;
		float y = cos(time * i * 1.28) * radius;
		float z = cos(time * i * 0.87) * radius;
		float sp1 = sphere(p + vec3(x, y, z), (sin(time + i) + 3.0) * 0.3 + volume);
		sp = smoothMin(sp, sp1, 5.0);
	}
	// return smoothMin(sp, min(fl, fl2), 5.0);
	return sp;
}

float scene3(vec3 p) {
	vec3 q = rotate(p, time * 0.1, vec3(0.0, 0.0, 1.0));
	q.z -= time;
	float b = box(trans(q), 0.75);
	float sp = sphere(trans(q), 0.5);
	return b * 0.5 + sp * 0.5;
}

float distanceFunc(vec3 p) {
	if(sceneMode == 0 || sceneMode == 1) return scene1(p);
	else if(sceneMode == 2 || sceneMode == 3) return scene2(p);
}

vec3 getNormal(vec3 p) {
	float d = 0.0001;
	return normalize(vec3(
		distanceFunc(p + vec3(d, 0.0, 0.0)) - distanceFunc(p + vec3(-d, 0.0, 0.0)),
		distanceFunc(p + vec3(0.0, d, 0.0)) - distanceFunc(p + vec3(0.0, -d, 0.0)),
		distanceFunc(p + vec3(0.0, 0.0, d)) - distanceFunc(p + vec3(0.0, 0.0, -d))
	));
}

float computeDepth(vec4 clipPos) {
	return ((clipPos.z / clipPos.w) + 1.0) * 0.5;
}

void main() {
	// fragment position
	// vec2 p = (vTexCoord * 2.0 - resolution) / min(resolution.x, resolution.y);
    vec2 p = (vTexCoord * 2.0 - 1.0);
	p.x *= (resolution.x / resolution.y);
	p.y *= -1.0;

	// camera
	vec3 cPos = camPos;
	vec3 cDir = -normalize(camPos);
	vec3 cUp = camUp;
	vec3 cSide = cross(cDir, cUp);
	float targetDepth = 1.0 / tan(fov * 0.5 * DEG2RAD);

	vec3 ray = normalize(cSide * p.x + cUp * p.y + cDir * targetDepth);

	// marching loop
	float distance = 0.0;
	float rLen = 0.0;
	vec3 rPos = cPos;

	for(int i = 0; i < 99; i++) {
		distance = distanceFunc(rPos);
		rLen += distance;
		rPos = cPos + ray * rLen;
	}

	// hit check
	if(abs(distance) < 0.001) {
		vec4 world = modelViewProjectionMatrix * vec4(rPos, 1.0);
		gPosition = vec4(world.xyz, 1.0);
		vec3 normal = mat3(modelViewMatrix) * getNormal(rPos);
		gNormal = vec4(normal, 1.0);
		// gColor = vec4(0.3, 0.6, 0.5, 1.0);
		float sin_val = sin(length(rPos.xz) * 20.0 + time * 5.0);
		vec3 color;
		if(sceneMode <= 3) {
			color = vec3(1.0) * sin_val;
		} else if(sceneMode == 3) {
			color = vec3(0.05) + mod(rPos.z * 0.2 + time * 3.0, 2.0) * 0.6;
		}
		// gColor = vec4(vec3(sin(rPos.y * 30.0 + time * 2.0)), 1.0);
		gColor = vec4(color, 1.0);

		float d = computeDepth(modelViewProjectionMatrix * vec4(rPos, 1.0));
		gl_FragDepth = d;
//		float zc = (modelViewProjectionMatrix * vec4(rPos, 1.0)).z;
//		float wc = (modelViewProjectionMatrix * vec4(rPos, 1.0)).w;
	} else {	
		gPosition = vec4(0.0, 0.0, 0.0, 1.0);
		gNormal = vec4(0.0, 0.0, 0.0, 1.0);
		gColor = vec4(0.0, 0.0, 0.0, 1.0);
		gl_FragDepth = 1.0;
	}
}
		
