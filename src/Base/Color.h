//
// Created by 王晨辉 on 2023/6/9.
//

#ifndef GAMEPHYSICSINONEWEEKEND_COLOR_H
#define GAMEPHYSICSINONEWEEKEND_COLOR_H
#include <Base/Log.h>

inline glm::vec3 ColorMap(int i, int min, int max) {
    float t = static_cast<float>(i - min) / (float)(max - min);
    float r = 0.0f, g = 0.0f, b = 0.0f;

    if (t < 0.2f) {
        r = 0.0f;
        g = 5.0f * t;
        b = 1.0f;
    } else if (t < 0.4f) {
        r = 0.0f;
        g = 1.0f;
        b = 1.0f - 5.0f * (t - 0.2f);
    } else if (t < 0.6f) {
        r = 5.0f * (t - 0.4f);
        g = 1.0f;
        b = 0.0f;
    } else if (t < 0.8f) {
        r = 1.0f;
        g = 1.0f - 5.0f * (t - 0.6f);
        b = 0.0f;
    } else {
        r = 1.0f;
        g = 0.0f;
        b = 5.0f * (t - 0.8f);
    }

    return {r, g, b};
}


#endif //GAMEPHYSICSINONEWEEKEND_COLOR_H
