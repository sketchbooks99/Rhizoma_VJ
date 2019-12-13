#version 450

uniform mat4 modelViewProjectionMatrix;
uniform vec3 scale;

in vec4 position;

struct Boids {
    vec3 pos;
    vec3 vel;
};

layout(std430, binding=1) buffer boids {
    Boids b[];
};

mat4 eulerAngleToRotationMatrix(vec3 angles) {
    float ch = cos(angles.y); float sh = sin(angles.y); // heading
    float ca = cos(angles.z); float sa = sin(angles.z); // attitude
    float cb = cos(angles.x); float sb = sin(angles.x); // bank

    // RyRxRz (Heading Bank Attitude)
    return mat4(
        ch * ca + sh * sb * sa, -ch * sa + sh * sb * ca, sh * cb, 0,
        cb * sa, cb * ca, -sb, 0,
        -sh * ca + ch * sb * sa, sh * sa + ch * sb * ca, ch * cb, 0,
        0, 0, 0, 1
    ); 
}

void main() {
    Boids boid = b[gl_InstanceID];
    mat4 obj2world = mat4(0.0);
    obj2world[0][0] = scale.x; obj2world[1][1] = scale.y; obj2world[2][2] = scale.z; obj2world[3][3] = 1.0;
    float rotY = -atan(boid.vel.x, boid.vel.z);
    float rotX = -asin(boid.vel.y / (length(boid.vel.xyz) + 1e-8));
    mat4 rotMatrix = eulerAngleToRotationMatrix(vec3(rotX, rotY, 0.0));
    obj2world = rotMatrix * obj2world;
    // obj2world[0][3] += boid.pos.x; 
    // obj2world[1][3] += boid.pos.y; 
    // obj2world[2][3] += boid.pos.z; 

    vec3 pos = (obj2world * position).xyz + boid.pos;

    gl_Position = vec4(pos, 1.0);
}