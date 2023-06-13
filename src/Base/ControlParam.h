//
// Created by 王晨辉 on 2023/6/10.
//

#ifndef GAMEPHYSICSINONEWEEKEND_CONTROL_H
#define GAMEPHYSICSINONEWEEKEND_CONTROL_H

#include <Base/Log.h>

enum BVHSplitStrategy : int{
    MID_X_ALIAS,
    MID_TRIANGLE_PARTITION
};

namespace control{
    extern vec4 clear_color ;
    extern vec3 ground_color ;
    extern vec3 ground_pos ;
    extern vec3 ground_halfSize ;
    extern int Show_Level ;
    extern BVHSplitStrategy bvh_strategy;

}



#endif //GAMEPHYSICSINONEWEEKEND_CONTROL_H
