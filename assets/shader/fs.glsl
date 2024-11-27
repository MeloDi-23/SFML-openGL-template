#version 330

in vec3 pos;
out vec4 FragColor; // Define a user-defined output variable
void main() {
    float grey = 0.4;
    FragColor = vec4(vec3(grey), 1.0);
}
