#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;


layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};

out vec2 outTexCoord;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    outTexCoord = vec2(aPos.x * 20, aPos.y * 20);
}