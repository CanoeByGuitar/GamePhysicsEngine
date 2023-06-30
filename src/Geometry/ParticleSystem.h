#ifndef GAMEPHYSICSINONEWEEKEND_PARTICESYSTEM_H
#define GAMEPHYSICSINONEWEEKEND_PARTICESYSTEM_H
#include <Base/Log.h>
#include "Geometry.h"

namespace geo{
    struct Particles3D{
        Particles3D(const std::vector<vec3>& pos):m_pos(pos){}


        std::vector<vec3> m_pos;
    };

    struct Particles2D{
        Particles2D(const std::vector<vec2>& pos):m_pos(pos){}
        std::vector<int> ComputeConvexHull();
        Particles3D to3D(float z = 0);
        std::vector<std::vector<vec3>> ComputeMultiLayerConvexHull();
        std::vector<std::vector<vec3>> ComputeMultiLayerConvexHullOn2();

        std::vector<vec2> m_pos;
    };

}

using GeoParticlesPtr = std::shared_ptr<geo::Particles3D>;




#endif



