//
// Created by 王晨辉 on 2023/6/10.
//

#include <Base/ControlParam.h>

namespace control{
    vec4 clear_color = vec4(0.45f, 0.55f, 0.60f, 1.00f);

    vec3 ground_color = {0.2, 0.4, 0.6};
    vec3 ground_pos = vec3(0, -1.2, 0);
    vec3 ground_halfSize = vec3(100, 1.2, 100);
    int Show_Level = 10;

    BVHSplitStrategy bvh_strategy = MID_TRIANGLE_PARTITION;

}