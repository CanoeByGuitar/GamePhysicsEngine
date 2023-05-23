//
// Created by 王晨辉 on 2023/4/29.
//

#ifndef GAMEPHYSICSINONEWEEKEND_UNIFIEDMODEL_H
#define GAMEPHYSICSINONEWEEKEND_UNIFIEDMODEL_H

#include <memory>
#include "glm/glm.hpp"
#include "BinReader.h"

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

struct Vertex{
    vec3 Position;
    vec3 Normal;
    vec2 UV;

    void ReadVertex(std::shared_ptr<BinReader> reader){
        Position = reader->Read<vec3>();
        Normal = reader->Read<vec3>();
        UV = reader->Read<vec2>();

        // TODO: other extra info
    }
};


struct UnifieldModel{


};

#endif //GAMEPHYSICSINONEWEEKEND_UNIFIEDMODEL_H
