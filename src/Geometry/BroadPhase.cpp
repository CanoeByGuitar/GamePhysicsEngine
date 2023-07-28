//
// Created by 王晨辉 on 2023/7/16.
//

#include "BroadPhase.h"
BroadPhase::BroadPhase(const std::vector<std::vector<unsigned int>>& gridObjects, const vec3& cellSize,
                 const GeoAabbPtr& worldSpace)
  : gridObjects(gridObjects)
  , cellSize(cellSize)
  , worldSpace(worldSpace) {
  PHY_ASSERT(cellSize.x <= 0 || cellSize.y <= 0 || cellSize.z <= 0, "CellSize error")
  invCellSize = 1.f / cellSize;
}
