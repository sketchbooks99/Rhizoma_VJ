#version 440
precision mediump float;

// #pragma include "../util/util.frag"

uniform float time;
uniform vec2 resolution;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform vec3 camPos;
uniform vec3 camUp;
uniform float fov;
uniform float farClip;
uniform float nearClip;
uniform bool isColored;
uniform vec3 edgeColor;

in vec2 vTexCoord;

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gColor;

const float PI = 3.14159265;
const float TWO_PI = PI * 2.0;
const float angle = 60.0;
const float DEG2RAD = (PI/180.0);

//===================================================

struct Object {
    float dist;
    vec3 color;
    vec3 edge;
};

float smoothMin(float d1, float d2, float k) {
    float h = exp(-k * d1) + exp(-k * d2);
    return -log(h) / k;
}

vec3 hsb2rgb( vec3 c ){
    vec3 rgb = clamp(abs(mod(c.x*6.0+vec3(0.0,4.0,2.0),
                    6.0)-3.0)-1.0,
                    0.0,
                    1.0 );
    rgb = rgb*rgb*(3.0-2.0*rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float snoise(vec2 v){
    const vec4 C = vec4(0.211324865405187, 0.366025403784439,
            -0.577350269189626, 0.024390243902439);
    vec2 i  = floor(v + dot(v, C.yy) );
    vec2 x0 = v -   i + dot(i, C.xx);
    vec2 i1;
    i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
    vec4 x12 = x0.xyxy + C.xxzz;
    x12.xy -= i1;
    i = mod(i, 289.0);
    vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
    + i.x + vec3(0.0, i1.x, 1.0 ));
    vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
        dot(x12.zw,x12.zw)), 0.0);
    m = m*m ;
    m = m*m ;
    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;
    m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
    vec3 g;
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * x12.xz + h.yz * x12.yw;
    return 130.0 * dot(m, g);
}

vec3 lightDir = normalize(vec3(0.577, 0.577, .577));
vec3 repeat(vec3 p, float interval) {
    return mod(p, interval) - interval * 0.5;
}

vec3 fold(vec3 p, int mode) {
    if(mode == 0) p.x = abs(p.x);
    else if(mode == 1) p.y = abs(p.y);
    else if(mode == 2) p.z = abs(p.z);
    return p;
}

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

vec3 twist(vec3 p, float power) {
    float s = sin(power * p.y);
    float c = cos(power * p.y);
    mat3 m = mat3(
        c, 0.0, -s,
        0.0, 1.0, 0.0,
        s, 0.0, c
    );
    return m * p;
}

mat2 rotate(float a) {
    float s = sin(a), c = cos(a);
    return mat2(c, s, -s, -c);
}

vec2 foldRotate(vec2 p, float s) {
    float a = PI / s - atan(p.y, p.x);
    float n = TWO_PI / s;
    a = float(a / n) * n;
    p *= rotate(a);
    return p;
}

float box(vec3 p, float size) {
    vec3 q = abs(p);
    return length(max(q - vec3(size), 0.0)) - size * 0.15;
}

float maxcomp(vec3 p) {
    float m1 = max(p.x, p.y);
    return max(m1, p.z);
}


vec2 obj_floor(in vec3 p)
{
  return vec2(p.y+10.0,0);
}

vec2 obj_box_s(vec3 p, vec3 b) {
    vec3 di = abs(p) - b;
    float mc = maxcomp(di);
    float d = min(mc, length(max(di, 0.0)));
    return vec2(d, 1.);
}

vec2 obj_box(vec3 p) {
    vec3 b = vec3(4., 4., 4.);
    return obj_box_s(p, b);
}

vec2 obj_cross(in vec3 p) {
    float inf = 100.;
    vec2 da = obj_box_s(p.xyz, vec3(inf, 2.0, 2.0));
    vec2 db = obj_box_s(p.xyz, vec3(2.0, inf, 2.0));
    vec2 dc = obj_box_s(p.xyz, vec3(2.0, 2.0, inf));
    return vec2(min(da.x, min(db.x, dc.x)), 1.);
}

vec2 obj_menger_simple(in vec3 p) {
    vec2 d1 = obj_box(p);
    vec2 d2 = obj_cross(p / 3.);
    float d = max(d1.x, -d2.x);
    return vec2(d, 1.0);
}

vec2 obj_menger(in vec3 p) {
    vec2 d2 = obj_box(p);
    float s = 1.2;
    for(int m=0; m<4; m++) {
        vec3 a = mod(p*s, 2.) - 1.;
        s *= 3.;
        vec3 r = 1. - 4. * abs(a);
        vec2 c = obj_cross(r)/s;
        d2.x = max(d2.x, c.x);
    }

    return d2;
}

float menger(vec3 z0, vec3 offset, float scale) {
    vec4 z = vec4(z0, 1.0);
    for(int i = 0; i < 4; i++) {
        z = abs(z);

        if(z.x < z.y) z.xy = z.yx;
        if(z.x < z.z) z.xz = z.zx;
        if(z.y < z.z) z.yz = z.zy;

        z *= scale;
        z.xyz -= offset * (scale - 1.0);

        if(z.z < -0.5 * offset.z * (scale - 1.0))
            z.z += offset.z * (scale - 1.0);
    }
    return (length(max(abs(z.xyz) - 
        vec3(1.0, 1.0, 1.0), 0.0)) - 0.05) / z.w;
}

Object distanceFunc(vec3 p) {
    float bx = box(rotate(p, time, vec3(1., 0.75, .53)), .05);
    vec3 q = p;
    // q.xy = foldRotate(q.xy, 5.0);
    q -= vec3(0.0, 0.0, time * 0.8);
    q = repeat(q, 5.);
    float gam_menger = menger(q * .3, vec3(.79, 1.1, .47), 2.31);
    q.yx = foldRotate(q.yx, 2.);
    float dist = max(gam_menger,obj_menger(q).x);
    float d = length(q - vec3(0.0, 0.0, q.z));
    vec3 bx_col = vec3(1., 0., 0.);
    Object obj;
    obj.dist = dist;
    vec3 col = vec3(.05) + mod(q.z * 0.2 + time * 1.5, 2.0) * 2.0;
    obj.color = col;
    obj.edge = vec3(hsb2rgb(vec3(mod(time, 1.0), 1.0, 1.0)));
    return obj;
}

vec3 getNormal(vec3 p) {
    float d = 0.001;
    return normalize(vec3(
        distanceFunc(p + vec3(d, 0.0, 0.0)).dist - distanceFunc(p + vec3(-d, 0.0, 0.0)).dist,
        distanceFunc(p + vec3(0.0, d, 0.0)).dist - distanceFunc(p + vec3(0.0, -d, 0.0)).dist,
        distanceFunc(p + vec3(0.0, 0.0, d)).dist - distanceFunc(p + vec3(0.0, 0.0, -d)).dist
    ));
}

float computeDepth(vec4 clipPos) {
	return ((clipPos.z / clipPos.w) + 1.0) * 0.5;
}

void main() {
    // vec2 p = (vTexCoord * 2.0 - resolution) / min(resolution.x, resolution.y);
    vec2 p = (vTexCoord * 2.0 - 1.0);
	p.y *= -1;

    float t = mod(time, 6.0);
    // vec3 cPos = vec3(sin(time * .5) * .5, .1, cos(time * .5) * .5);
    vec3 cPos = normalize(camPos) * 2.0;
	vec3 cDir = -normalize(camPos);
	vec3 cUp = camUp;
	vec3 cSide = cross(cDir, cUp);
	float targetDepth = 1.0 / tan(fov * 0.5 * DEG2RAD);

	vec3 ray = normalize(cSide * p.x + cUp * p.y + cDir * targetDepth);

    float rLen = 0.0;
    vec3 rPos = cPos;
    Object obj;
    obj.dist = 0.0;
    for(int i=0; i<128; i++) {
        obj = distanceFunc(rPos);
        rLen += obj.dist;
        rPos = cPos + ray * rLen;
        if(obj.dist < 0.0001) break;
    }
    
    if(obj.dist < 0.0001) {
		vec3 normal = getNormal(rPos);
       float eps = 0.001;
       vec3 dif_x = getNormal(rPos+ vec3(eps, 0.0, 0.0));
       vec3 dif_y = getNormal(rPos + vec3(0.0, eps, 0.0));
       vec3 dif_z = getNormal(rPos + vec3(0.0, 0.0, eps));

       float max_dif = .99;
       
        if(isColored) {
            if(abs(dot(normal, dif_x)) < max_dif ||
            abs(dot(normal, dif_y)) < max_dif || 
            abs(dot(normal, dif_z)) < max_dif) {
                    // obj.color = vec3(abs(dot(normal, dif_x)), abs(dot(normal, dif_y)), abs(dot(normal, dif_z)));
                obj.color = obj.edge;
            }
        }
		vec4 world = modelViewMatrix * vec4(rPos, 1.0);
		gPosition = vec4(world.xyz, 1.0);
		gNormal = vec4(mat3(modelViewMatrix) * normal, 1.0);
		gColor = vec4(obj.color, 1.0);

		float d = computeDepth(modelViewProjectionMatrix * vec4(rPos, 1.0));
		gl_FragDepth = d;
	}
}