#version 460

layout(location = 0) in vec3 xyz;

void main() {
    gl_Position = vec4(xyz, 1.0);
}