//
// Created by 王晨辉 on 2023/7/28.
//

#include "TetBoolean.h"
#include "TetrahedronIterator.h"
#include <set>


SimpleComplex::TetrahedronMesh SimpleComplex::TetBoolean::Union(
  const SimpleComplex::TetrahedronMesh& lhs, const SimpleComplex::TetrahedronMesh& rhs) {}

SimpleComplex::TetrahedronMesh SimpleComplex::TetBoolean::Intersection(
  const SimpleComplex::TetrahedronMesh& lhs, const SimpleComplex::TetrahedronMesh& rhs) {}

SimpleComplex::TetrahedronMesh SimpleComplex::TetBoolean::Difference(
  const SimpleComplex::TetrahedronMesh& lhs, const SimpleComplex::TetrahedronMesh& rhs) {}


SimpleComplex::TetBoolean::TwoVecInt SimpleComplex::TetBoolean::AInB(
  SimpleComplex::TetrahedronMesh& A, SimpleComplex::TetrahedronMesh& B) {
  // tetMesh A and B will be changed
  GetIntersectionLine(A, B);

  std::vector<int> vertexInNewMesh;
  std::vector<int> faceInNewMesh;
  std::set<int>   verC;
  for (VertexIterator it(A); !it.Done(); it.Advance()) {
    if (m_vertexFlag[it.Idx()] == 0 && !vertexInNewMesh[it.Idx()]) {
      verC.insert(it.Idx());
      for (auto v_c : verC) {
        // neighbour vertices
        for (int v_hat : A.m_VV[v_c]) {
          if (verC.find(v_hat) == verC.end() && !m_vertexOnIntersection[v_hat]) {
            vertexInNewMesh[v_hat] = 1;
            verC.insert(v_hat);
          }
        }

        // neighbour triangles
        for(int t_hat : A.m_VF[v_c]){
          faceInNewMesh[t_hat] = 1;
        }

        verC.erase(v_c);
      }
    }
  }
  return {vertexInNewMesh, faceInNewMesh};
}

void SimpleComplex::TetBoolean::GetIntersectionLine(const SimpleComplex::TetrahedronMesh& A,
                                                    const SimpleComplex::TetrahedronMesh& B) {
  // this will change the topology of A and B
  // (1) get Intersection point
  // (2) connect points to lines
  // (3) re-triangulate
  // (4) update tet data structure

}


SimpleComplex::TetrahedronMesh SimpleComplex::TetBoolean::GetTetMesh(const SimpleComplex::TetBoolean::TwoVecInt& twoVecInt) {
  // should be easy
  // when loading tetMesh from txt
  // 4 triangles of one tet should be consecutive
  // twoVecInt should be like
  // (1)  face :[0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0]
  // (2)  vertices: [...]

}
