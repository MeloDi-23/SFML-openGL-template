#version 330

in vec3 pos;
out vec4 FragColor; // Define a user-defined output variable
void main() {
    FragColor = vec4(pos*0.5 + vec3(0.5, 0.5, 0.5), 1.0);
    // FragColor = vec4(abs(pos), 1.0);
}
