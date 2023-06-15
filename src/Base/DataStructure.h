//
// Created by 王晨辉 on 2023/6/15.
//

#ifndef GAMEPHYSICSINONEWEEKEND_DATASTRUCTURE_H
#define GAMEPHYSICSINONEWEEKEND_DATASTRUCTURE_H

#include <cstdio>
#include <Base/Log.h>

struct Vec3Hash{
    size_t operator()(const vec3& v) const{
        std::size_t h1 = std::hash<float>{}(v.x);
        std::size_t h2 = std::hash<float>{}(v.y);
        std::size_t h3 = std::hash<float>{}(v.z);
        return h1 ^ h2 ^ h3;
    }
};

#endif //GAMEPHYSICSINONEWEEKEND_DATASTRUCTURE_H
