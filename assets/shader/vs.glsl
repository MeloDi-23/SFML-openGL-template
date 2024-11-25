#version 330
uniform mat4 model;
uniform mat4 cam;

layout(location=0) in vec3 a_Pos;
out vec3 pos;

void main() {
    gl_Position = cam*model*vec4(a_Pos, 1.0);
    pos = a_Pos;
}

