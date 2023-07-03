#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;

out vec2 TexCoord;

layout (std140) uniform Matrices {
    mat4 projection;
    mat4 view;
};



void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}