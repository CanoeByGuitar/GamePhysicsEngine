//
// Created by 王晨辉 on 2023/6/15.
//

#ifndef GAMEPHYSICSINONEWEEKEND_DATASTRUCTURE_H
#define GAMEPHYSICSINONEWEEKEND_DATASTRUCTURE_H

#include <Base/Log.h>
#include <cstdio>


struct Vec3Hash {
    size_t operator()(const vec3& v) const {
        size_t seed = 0;
        std::hash<float> hasher;
        seed ^= hasher(v.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(v.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

#endif  //GAMEPHYSICSINONEWEEKEND_DATASTRUCTURE_H
