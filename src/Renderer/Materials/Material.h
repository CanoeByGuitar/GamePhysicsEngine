//
// Created by 王晨辉 on 2023/7/1.
//

#ifndef GAMEPHYSICSINONEWEEKEND_MATERIAL_H
#define GAMEPHYSICSINONEWEEKEND_MATERIAL_H

#include <Base/Log.h>
#include "../stb_image.h"

inline unsigned int LoadTexture(const std::string& path){
    std::string filename = std::string(path);

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data){
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }else{
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }
    return textureID;
}

class Material {
   public:
    Material() = default;

    void Load(){

    }

    void Init(int texture, const vec3& color, float alpha){
        m_texture = texture;
        m_color = color;
        m_alpha = alpha;
    };



   public:
    unsigned int m_texture;
    vec3 m_color;
    float m_alpha;
    std::string m_texturePath;
    int m_indexOfModel; // the 2nd mesh of a model should have the 2nd material
};
using MaterialPtr = std::shared_ptr<Material>;

#endif  //GAMEPHYSICSINONEWEEKEND_MATERIAL_H
