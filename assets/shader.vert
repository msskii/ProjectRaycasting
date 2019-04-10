//vertex shader
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 inColor;

uniform vec3 test;

out vec3 vertColor;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertColor = inColor;
}
