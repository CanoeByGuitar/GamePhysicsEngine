#version 330 core

out vec4 FragColor;

uniform vec3 color;

void main() {
//    FragColor =  vec4(vec3(227,75,33) / 255.f, 1.0f);
    FragColor = vec4(color, 1.0f);
}