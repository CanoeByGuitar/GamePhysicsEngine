#version 330 core

in vec3 normal;
out vec4 FragColor;

uniform vec3 color;



void main() {
    //    FragColor =  vec4(1.0f, 0.f, 0.f , 1.0f);
    vec3 normalColor = color;
    normalColor = (normal + vec3(1, 1, 1)) * 0.5;
    FragColor = vec4(normalColor, 1.0f);
}
