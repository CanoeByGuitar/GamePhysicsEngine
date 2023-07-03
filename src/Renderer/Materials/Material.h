//
// Created by 王晨辉 on 2023/7/1.
//

#ifndef GAMEPHYSICSINONEWEEKEND_MATERIAL_H
#define GAMEPHYSICSINONEWEEKEND_MATERIAL_H

#include <Base/Log.h>
#include "../stb_image.h"
#include <glad/glad.h>

class Material {
   public:
    Material() = default;

    void LoadTexture();

    void Init(int texture, const vec3& color, float alpha){
        m_texture = texture;
        m_color = color;
        m_alpha = alpha;
    };



   public:
    unsigned int m_texture = 0;
    vec3 m_color = vec3{0};
    float m_alpha;
    std::string m_texturePath;
    int m_indexOfModel; // the 2nd mesh of a model should have the 2nd material
};
using MaterialPtr = std::shared_ptr<Material>;

#endif  //GAMEPHYSICSINONEWEEKEND_MATERIAL_H
