#version 330 core

out vec4 fragColor;
uniform vec3 color;

in vec2 outTexCoord;

void main() {
    vec3 tileColor = color;
    vec3 stripeColor = vec3(0.2, 0.2, 0.2);
    float stripeWidth = 5;
    // 获取表面纹理坐标
    vec2 texCoord = outTexCoord;

    // 计算条纹模式
    float stripe = mod(floor(texCoord.x / stripeWidth) + floor(texCoord.y / stripeWidth), 2.0);

    // 根据条纹模式设置颜色
    vec3 temp_color = mix(tileColor, stripeColor, stripe);

    // 输出最终颜色
    fragColor = vec4(temp_color, 1.0);
}
