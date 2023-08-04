//
// Created by 王晨辉 on 2023/7/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_TETBOOLEAN_H
#define GAMEPHYSICSINONEWEEKEND_TETBOOLEAN_H


#include "TetrahedronMesh.h"
#include <poly2tri.h>

#include <utility>
#include <Base/DataStructure.h>

namespace SimpleComplex {
struct BooleanTet {
  TetrahedronMesh* m_tetMesh;
  std::vector<int> m_vertexFlag;
  //  0: in
  //  1: out
  //  2: on
  //  -1: else

  std::vector<std::vector<int>> m_faceSteiner;

  // for each face: {vec3 ==> vertex ID}
  std::vector<std::unordered_map<vec3, int, Vec3Hash>> m_faceSteinerHash;

  int m_originFaceNum;

  explicit BooleanTet(TetrahedronMesh* tet)
    : m_tetMesh(tet) {
    m_vertexFlag  = std::vector<int>(m_tetMesh->m_vertices.size(), -1);
    m_faceSteiner = std::vector<std::vector<int>>(m_tetMesh->m_faces.size());
    m_faceSteinerHash = std::vector<std::unordered_map<vec3, int, Vec3Hash>>(m_tetMesh->m_faces.size());
  }

  BooleanTet(BooleanTet&& other) noexcept
    : m_tetMesh(other.m_tetMesh)
    , m_vertexFlag(std::move(other.m_vertexFlag))
    , m_faceSteiner(std::move(other.m_faceSteiner))
    , m_faceSteinerHash(std::move(other.m_faceSteinerHash)) {
    other.m_tetMesh = nullptr;
  }
};

class TetBoolean {
public:
  TetBoolean(const TetBoolean&)            = delete;
  TetBoolean& operator=(const TetBoolean&) = delete;

  static TetBoolean& GetInstance(TetrahedronMesh* A, TetrahedronMesh* B) {
    static TetBoolean instance(A, B);
    return instance;
  }

  TetrahedronMesh Union();
  TetrahedronMesh Intersect();
  TetrahedronMesh Difference();

  using TwoVecInt = std::pair<std::vector<int>, std::vector<int>>;

  TwoVecInt       AInB();
  TetrahedronMesh AOutB();

  void GetIntersectionLine();

  TetrahedronMesh GetTetMesh(const TwoVecInt& twoVecInt);

  static void Triangulate(SimpleComplex::TetrahedronMesh& tetMesh,
                          std::vector<std::vector<int>>&  faceSteiner,
                          int originFaceNum);

private:
  TetBoolean(TetrahedronMesh* A, TetrahedronMesh* B);
  BooleanTet m_A, m_B;
};
}   // namespace SimpleComplex


#endif   // GAMEPHYSICSINONEWEEKEND_TETBOOLEAN_H
