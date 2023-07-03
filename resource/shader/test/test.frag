#version 330 core

out vec4 FragColor;

uniform vec3 color;
uniform sampler2D texSampler;

void main() {
    FragColor =  vec4(1.0f, 0.f, 0.f , 1.0f);
//    FragColor = vec4(color, 1.0f);
}
