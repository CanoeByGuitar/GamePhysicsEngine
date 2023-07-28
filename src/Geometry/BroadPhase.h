//
// Created by 王晨辉 on 2023/7/16.
//

#ifndef GAMEPHYSICSINONEWEEKEND_BROADPHASE_H
#define GAMEPHYSICSINONEWEEKEND_BROADPHASE_H

#include "Geometry.h"
class BroadPhase {

public:
  BroadPhase(const std::vector<std::vector<unsigned int>>& gridObjects, const vec3& cellSize,
             const GeoAabbPtr& worldSpace);

  std::vector<unsigned int> at(int i, int j, int k);


private:
  std::vector<std::vector<unsigned int>> gridObjects;
  vec3                                   cellSize;
  vec3                                   invCellSize;
  GeoAabbPtr                             worldSpace;
};



#endif   // GAMEPHYSICSINONEWEEKEND_BROADPHASE_H
