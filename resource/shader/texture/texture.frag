#version 330 core
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 color;
uniform sampler2D texSampler;

void main() {

    FragColor = texture(texSampler, TexCoord) * vec4(color, 1.0f);
}
