#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

layout(push_constant) uniform PushData{
	mat2 transform;
	vec2 offset;
	vec3 color;
} pushData;

void main() {
    gl_Position = vec4(position, 0.0, 1.0);
}