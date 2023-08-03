//
// Created by 王晨辉 on 2023/7/28.
//

#include "TetBoolean.h"
#include "TetrahedronIterator.h"
#include <set>


SimpleComplex::TetrahedronMesh SimpleComplex::TetBoolean::Union(
  const SimpleComplex::TetrahedronMesh& lhs, const SimpleComplex::TetrahedronMesh& rhs) {}

SimpleComplex::TetrahedronMesh SimpleComplex::TetBoolean::Intersect(
  const SimpleComplex::TetrahedronMesh& lhs, const SimpleComplex::TetrahedronMesh& rhs) {}

SimpleComplex::TetrahedronMesh SimpleComplex::TetBoolean::Difference(
  const SimpleComplex::TetrahedronMesh& lhs, const SimpleComplex::TetrahedronMesh& rhs) {}


SimpleComplex::TetBoolean::TwoVecInt SimpleComplex::TetBoolean::AInB(
  SimpleComplex::TetrahedronMesh& A, SimpleComplex::TetrahedronMesh& B) {
  // tetMesh A and B will be changed
  GetIntersectionLine(A, B);

  std::vector<int> vertexInNewMesh;
  std::vector<int> faceInNewMesh;
  std::set<int>    verC;
  for (VertexIterator it(A); !it.Done(); it.Advance()) {
    if (m_vertexFlagA[it.Idx()] == 0 && !vertexInNewMesh[it.Idx()]) {
      verC.insert(it.Idx());
      for (auto v_c : verC) {
        // neighbour vertices
        for (int v_hat : A.m_VV[v_c]) {
          if (verC.find(v_hat) == verC.end() && m_vertexFlagA[v_hat] != 2) {
            vertexInNewMesh[v_hat] = 1;
            verC.insert(v_hat);
          }
        }

        // neighbour triangles
        for (int t_hat : A.m_VF[v_c]) {
          faceInNewMesh[t_hat] = 1;
        }

        verC.erase(v_c);
      }
    }
  }
  return {vertexInNewMesh, faceInNewMesh};
}

void SimpleComplex::TetBoolean::GetIntersectionLine(SimpleComplex::TetrahedronMesh& A,
                                                    SimpleComplex::TetrahedronMesh& B) {
  // this will change the topology of A and B
  // (1) get Intersection point
  // (2) connect points to lines
  // (3) re-triangulate
  // (4) update tet data structure
  for (FaceIterator faceIterA(A); !faceIterA.Done(); faceIterA.Advance()) {
    for (FaceIterator faceIterB(B); !faceIterB.Done(); faceIterB.Advance()) {
      Face                      T1 = faceIterA.Current();
      Face                      T2 = faceIterB.Current();

      vec3 pA0 = A.m_vertices[T1.m_verts[0]].ToVec3();
      vec3 pA1 = A.m_vertices[T1.m_verts[1]].ToVec3();
      vec3 pA2 = A.m_vertices[T1.m_verts[2]].ToVec3();
      vec3 pB0 = B.m_vertices[T2.m_verts[0]].ToVec3();
      vec3 pB1 = B.m_vertices[T2.m_verts[1]].ToVec3();
      vec3 pB2 = B.m_vertices[T2.m_verts[2]].ToVec3();
      std::vector<Intersection> inters;   // should be 2 or 0
      for (EdgeIterator edgeIter(A); !edgeIter.Done(); edgeIter.Advance()) {
        auto edge  = edgeIter.Current();
        vec3 o     = A.m_vertices[edge.m_verts[0]].ToVec3();
        vec3 end   = A.m_vertices[edge.m_verts[1]].ToVec3();
        auto inter = GetIntersection(o, end, pB0, pB1, pB2);
        if (inter.m_isHit){
          inters.push_back(inter);
        }

      }

      for (EdgeIterator edgeIter(B); !edgeIter.Done(); edgeIter.Advance()) {
        auto edge  = edgeIter.Current();
        vec3 o     = B.m_vertices[edge.m_verts[0]].ToVec3();
        vec3 end   = B.m_vertices[edge.m_verts[1]].ToVec3();
        auto inter = GetIntersection(o, end, pA0, pA1, pA2);
        if (inter.m_isHit)
          inters.push_back(inter);
      }

      if (inters.empty())
        continue;
      else {
        int a0 = A.AddVertex(inters[0].m_hitPoint);
        int a1 = A.AddVertex(inters[1].m_hitPoint);
        int b0 = B.AddVertex(inters[0].m_hitPoint);
        int b1 = B.AddVertex(inters[1].m_hitPoint);
        m_vertexFlagA[a0] = 2;
        m_vertexFlagA[a1] = 2;
        m_vertexFlagB[b0] = 2;
        m_vertexFlagB[b1] = 2;
        A.AddEdge(a0, a1);
        B.AddEdge(b0, b1);

        // classify 6 points flag of 2 triangles


        // re-triangulated

      }
    }
  }
}


SimpleComplex::TetrahedronMesh SimpleComplex::TetBoolean::GetTetMesh(
  const SimpleComplex::TetBoolean::TwoVecInt& twoVecInt) {
  // should be easy
  // when loading tetMesh from txt
  // 4 triangles of one tet should be consecutive
  // twoVecInt should be like
  // (1)  face :[0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0]
  // (2)  vertices: [...]
}
