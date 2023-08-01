//
// Created by 王晨辉 on 2023/7/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_TETBOOLEAN_H
#define GAMEPHYSICSINONEWEEKEND_TETBOOLEAN_H


#include "TetrahedronMesh.h"

namespace SimpleComplex{
class TetBoolean{
public:
  TetBoolean(const TetBoolean&) = delete;
  TetBoolean& operator=(const TetBoolean&) = delete;

  static TetBoolean& GetInstance(){
    static TetBoolean instance;
    return instance;
  }

  TetrahedronMesh Union(const TetrahedronMesh& lhs, const TetrahedronMesh& rhs);
  TetrahedronMesh Intersection(const TetrahedronMesh& lhs, const TetrahedronMesh& rhs);
  TetrahedronMesh Difference(const TetrahedronMesh& lhs, const TetrahedronMesh& rhs);


private:
  TetBoolean(){}
};
}


#endif   // GAMEPHYSICSINONEWEEKEND_TETBOOLEAN_H
