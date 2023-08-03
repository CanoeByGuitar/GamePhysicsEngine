//
// Created by 王晨辉 on 2023/7/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_TETBOOLEAN_H
#define GAMEPHYSICSINONEWEEKEND_TETBOOLEAN_H


#include "TetrahedronMesh.h"
#include <poly2tri.h>

namespace SimpleComplex {
class TetBoolean {
public:
  TetBoolean(const TetBoolean&)            = delete;
  TetBoolean& operator=(const TetBoolean&) = delete;

  static TetBoolean& GetInstance() {
    static TetBoolean instance;
    return instance;
  }

  TetrahedronMesh Union(const TetrahedronMesh& lhs, const TetrahedronMesh& rhs);
  TetrahedronMesh Intersect(const TetrahedronMesh& lhs, const TetrahedronMesh& rhs);
  TetrahedronMesh Difference(const TetrahedronMesh& lhs, const TetrahedronMesh& rhs);

  using TwoVecInt = std::pair<std::vector<int>, std::vector<int>>;

  TwoVecInt       AInB(TetrahedronMesh& A, TetrahedronMesh& B);
  TetrahedronMesh AOutB(TetrahedronMesh& A, TetrahedronMesh& B);

  void GetIntersectionLine(SimpleComplex::TetrahedronMesh& A, SimpleComplex::TetrahedronMesh& B);

  TetrahedronMesh GetTetMesh(const TwoVecInt& twoVecInt);

  static void Triangulate(SimpleComplex::TetrahedronMesh& tetMesh,
                   std::vector<std::vector<int>>& faceSteiner);

private:
  TetBoolean();

  //  0: in
  //  1: out
  //  2: on
  //  -1: else
  // TODO: initialize with -1
  std::vector<int> m_vertexFlagA;
  std::vector<int> m_vertexFlagB;

  std::vector<std::vector<int>> m_faceSteinerA;
  std::vector<std::vector<int>> m_faceSteinerB;



};
}   // namespace SimpleComplex


#endif   // GAMEPHYSICSINONEWEEKEND_TETBOOLEAN_H
