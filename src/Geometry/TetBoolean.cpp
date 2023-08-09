//
// Created by 王晨辉 on 2023/7/28.
//

#include "TetBoolean.h"
#include "TetrahedronIterator.h"
#include <set>

using p2t::CDT;

namespace SimpleComplex {
TetBoolean::TetBoolean(TetrahedronMesh* A, TetrahedronMesh* B)
  : m_A(std::move(BooleanTet(A)))
  , m_B(std::move(BooleanTet(B))) {
  m_A.m_originFaceNum = (int)m_A.m_tetMesh->m_faces.size();
  m_B.m_originFaceNum = (int)m_B.m_tetMesh->m_faces.size();
}

void TetBoolean::GetIntersectionLine() {
  // this will change the topology of A and B
  // (1) get Intersection point
  // (2) connect points to lines
  // (3) re-triangulate
  // (4) update tet data structure
  TetrahedronMesh* A           = m_A.m_tetMesh;
  TetrahedronMesh* B           = m_B.m_tetMesh;
  auto             steinerA    = m_A.m_faceSteiner;
  auto             steinerB    = m_B.m_faceSteiner;
  auto             vertexFlagA = m_A.m_vertexFlag;
  auto             vertexFlagB = m_B.m_vertexFlag;

  PHY_DEBUG("there is totally {} triangles", m_A.m_tetMesh->m_faces.size());
  int faceCntA = -1;
  int faceCntB = 0;
  for (FaceIterator faceIterA(*A); !faceIterA.Done(); faceIterA.Advance()) {
    faceCntA++;
    faceCntB = 0;
    for (FaceIterator faceIterB(*B); !faceIterB.Done(); faceIterB.Advance()) {
      PHY_DEBUG("A face: {},  B face: {}", faceCntA, faceCntB++);
      Face T1 = faceIterA.Current();
      Face T2 = faceIterB.Current();

      vec3                      pA0 = A->m_vertices[T1.m_verts[0]].ToVec3();
      vec3                      pA1 = A->m_vertices[T1.m_verts[1]].ToVec3();
      vec3                      pA2 = A->m_vertices[T1.m_verts[2]].ToVec3();
      vec3                      pB0 = B->m_vertices[T2.m_verts[0]].ToVec3();
      vec3                      pB1 = B->m_vertices[T2.m_verts[1]].ToVec3();
      vec3                      pB2 = B->m_vertices[T2.m_verts[2]].ToVec3();
      std::vector<Intersection> inters;   // should be 2 or 0
      for (EdgeIterator edgeIter(*A); !edgeIter.Done(); edgeIter.Advance()) {
        auto edge  = edgeIter.Current();
        vec3 o     = A->m_vertices[edge.m_verts[0]].ToVec3();
        vec3 end   = A->m_vertices[edge.m_verts[1]].ToVec3();
        auto inter = GetIntersection(o, end, pB0, pB1, pB2);
        if (inter.m_isHit) {
          inters.push_back(inter);
          auto  hitPoint = inter.m_hitPoint;
          auto& mp       = m_B.m_faceSteinerHash;
          if (mp[faceIterB.Idx()].find(hitPoint) == mp[faceIterB.Idx()].end()) {
            int idx                       = B->AddVertex(inter.m_hitPoint);
            mp[faceIterB.Idx()][hitPoint] = idx;
            steinerB[faceIterB.Idx()].push_back(idx);
          } else {
            //            steinerB[faceIterB.Idx()].push_back(mp[faceIterB.Idx()][hitPoint]);
          }
        }
      }

      for (EdgeIterator edgeIter(*B); !edgeIter.Done(); edgeIter.Advance()) {
        auto edge  = edgeIter.Current();
        vec3 o     = B->m_vertices[edge.m_verts[0]].ToVec3();
        vec3 end   = B->m_vertices[edge.m_verts[1]].ToVec3();
        auto inter = GetIntersection(o, end, pA0, pA1, pA2);
        if (inter.m_isHit) {
          inters.push_back(inter);
          auto  hitPoint = inter.m_hitPoint;
          auto& mp       = m_A.m_faceSteinerHash;
          if (mp[faceIterA.Idx()].find(hitPoint) == mp[faceIterA.Idx()].end()) {
            int idx                       = A->AddVertex(inter.m_hitPoint);
            mp[faceIterA.Idx()][hitPoint] = idx;
            steinerA[faceIterA.Idx()].push_back(idx);
          } else {
            //            steinerA[faceIterA.Idx()].push_back(mp[faceIterA.Idx()][hitPoint]);
          }
        }
      }


      // classify 6 points flag of 2 triangles
      // verts of A to B
      for (int vert : T1.m_verts) {
        auto normal = T2.m_normal;
        auto f_x    = B->m_vertices[T2.m_verts[0]].ToVec3();
        auto v_x    = A->m_vertices[vert].ToVec3();
        auto temp   = glm::dot(f_x - v_x, normal);
        if (temp < 0) {
          vertexFlagA[vert] = 0;
        } else if (temp == 0) {
          vertexFlagA[vert] = 2;
        } else {
          if (!inters.empty()) {
            vertexFlagA[vert] = 1;
          }
        }
      }
      // verts of B to A
      for (int vert : T2.m_verts) {
        auto normal = T1.m_normal;
        auto f_x    = A->m_vertices[T1.m_verts[0]].ToVec3();
        auto v_x    = B->m_vertices[vert].ToVec3();
        auto temp   = glm::dot(f_x - v_x, normal);
        if (temp < 0) {
          vertexFlagB[vert] = 0;
        } else if (temp == 0) {
          vertexFlagB[vert] = 2;
        } else {
          if (!inters.empty()) {
            vertexFlagB[vert] = 1;
          }
        }
      }

      // get intersection line to visualization
      for (auto inter : inters) {
        //        PHY_DEBUG("inter: {}", inter.m_hitPoint);
        m_intersectionLine.push_back(inter.m_hitPoint);
      }
    }
  }

  // re-triangulated
  Triangulate(*A, steinerA, m_A.m_originFaceNum);
  Triangulate(*B, steinerB, m_B.m_originFaceNum);
}

std::vector<vec3> TetBoolean::GetIntersectionLineVertices() {
  auto              A = m_A.m_tetMesh;
  std::vector<vec3> ret;
  for (int i = m_A.m_originFaceNum; i < A->m_vertices.size(); i++) {
    ret.push_back(A->m_vertices[i].ToVec3());
  }
  return ret;
}



TetBoolean::TwoVecInt SimpleComplex::TetBoolean::AInB() {
  auto A = m_A.m_tetMesh;
  auto B = m_B.m_tetMesh;
  // tetMesh A and B will be changed
  GetIntersectionLine();
  m_A.m_vertexFlag.resize(m_A.m_tetMesh->m_vertices.size());

  std::vector<int> vertexInNewMesh(m_A.m_tetMesh->m_vertices.size());
  std::vector<int> faceInNewMesh;
  std::set<int>    verC;
  for (VertexIterator it(*A); !it.Done(); it.Advance()) {
    if (m_A.m_vertexFlag[it.Idx()] == 0 && !vertexInNewMesh[it.Idx()]) {
      verC.insert(it.Idx());
      for (auto v_c : verC) {
        // neighbour vertices
        for (int v_hat : A->m_VV[v_c]) {
          if (verC.find(v_hat) == verC.end() && m_A.m_vertexFlag[v_hat] != 2) {
            vertexInNewMesh[v_hat] = 1;
            verC.insert(v_hat);
          }
        }

        // neighbour triangles
        for (int t_hat : A->m_VF[v_c]) {
          faceInNewMesh.push_back(t_hat);
        }

        verC.erase(v_c);
      }
    }
  }
  return {vertexInNewMesh, faceInNewMesh};
}


GeoMeshPtr TetBoolean::ToGeoMeshAinB(const std::vector<int>& faceInNewMesh) {
  auto mesh = std::make_shared<geo::Mesh>();
  int  cnt  = 0;
  for (auto i : faceInNewMesh) {
    for (int j = 0; j < 3; j++) {
      auto& tet = m_A.m_tetMesh;
      mesh->vertices.push_back(tet->m_vertices[tet->m_faces[i].m_verts[j]].ToVec3());
      mesh->indices.push_back(cnt);
      cnt++;
    }
  }
  return mesh;
}

void TetBoolean::Triangulate(SimpleComplex::TetrahedronMesh& tetMesh,
                             std::vector<std::vector<int>>& faceSteiner, int originFaceNum) {
  for (FaceIterator it(tetMesh); !it.Done() && it.Idx() < originFaceNum; it.Advance()) {
    if (faceSteiner[it.Idx()].empty())
      continue;

    auto        p0 = tetMesh.m_vertices[it.Current().m_verts[0]].ToVec3();
    auto        p1 = tetMesh.m_vertices[it.Current().m_verts[1]].ToVec3();
    auto        p2 = tetMesh.m_vertices[it.Current().m_verts[2]].ToVec3();
    p2t::Point *a, *b, *c;
    int         projectType = 0;
    if (p0.x == p1.x && p0.x == p2.x) {
      // project to yoz

    } else if (p0.z == p1.z && p0.z == p2.z) {
      // project to xoy
      projectType = 1;
    } else {
      // project to xoz
      projectType = 2;
    }

    std::unordered_map<p2t::Point*, int> mp;
    if (projectType == 0) {
      a = new p2t::Point{p0.y, p0.z};
      b = new p2t::Point{p1.y, p1.z};
      c = new p2t::Point{p2.y, p2.z};
    } else if (projectType == 1) {
      a = new p2t::Point{p0.x, p0.y};
      b = new p2t::Point{p1.x, p1.y};
      c = new p2t::Point{p2.x, p2.y};
    } else {
      a = new p2t::Point{p0.x, p0.z};
      b = new p2t::Point{p1.x, p1.z};
      c = new p2t::Point{p2.x, p2.z};
    }
    mp[a] = it.Current().m_verts[0];
    mp[b] = it.Current().m_verts[1];
    mp[c] = it.Current().m_verts[2];

    std::vector<p2t::Point*> polyline{a, b, c};
    std::vector<p2t::Point*> steiner;

    for (auto pointId : faceSteiner[it.Idx()]) {
      auto pos = tetMesh.m_vertices[pointId].ToVec3();
      if (projectType == 0) {
        auto p = new p2t::Point(pos.y, pos.z);
        steiner.push_back(p);
        mp[p] = pointId;
      } else if (projectType == 1) {
        auto p = new p2t::Point(pos.x, pos.y);
        steiner.push_back(p);
        mp[p] = pointId;
      } else {
        auto p = new p2t::Point(pos.x, pos.z);
        steiner.push_back(p);
        mp[p] = pointId;
      }
    }

    CDT* cdt = new CDT(polyline);
    for (auto s : steiner) {
      cdt->AddPoint(s);
    }


    PHY_INFO("Poly Line: ({}, {}), ({}, {}), ({}, {}) \n",
             polyline[0]->x,
             polyline[0]->y,
             polyline[1]->x,
             polyline[1]->y,
             polyline[2]->x,
             polyline[2]->y);
    float eps = 1e-3;
    vec2 x2d(polyline[0]->x, polyline[0]->y);
    vec2 y2d(polyline[1]->x, polyline[1]->y);
    vec2 z2d(polyline[2]->x, polyline[2]->y);
    if (glm::all(glm::epsilonEqual(
          x2d, y2d, eps)) ||
        glm::all(glm::epsilonEqual(
          z2d, y2d, eps)) ||
        glm::all(glm::epsilonEqual(
          x2d, z2d, eps))
            ) {
      PHY_DEBUG("delete {}, {}, {}", x2d, y2d, z2d);
      return;
    }

    for (auto item : steiner) {
      PHY_INFO("steiner: ({},{}) \n", item->x, item->y);
    }

    try{
      cdt->Triangulate();
    }catch (const std::exception& e) {
      return;
    }catch (const std::runtime_error& e){
      return;
    }

    std::vector<p2t::Triangle*> triangles = cdt->GetTriangles();
    for (auto tri : triangles) {
      p2t::Point* pa = tri->GetPoint(0);
      p2t::Point* pb = tri->GetPoint(1);
      p2t::Point* pc = tri->GetPoint(2);
      int         e0 = tetMesh.AddEdge(mp[pa], mp[pb]);
      int         e1 = tetMesh.AddEdge(mp[pb], mp[pc]);
      int         e2 = tetMesh.AddEdge(mp[pc], mp[pa]);

      int f0 = tetMesh.AddFace(mp[pa], mp[pb], mp[pc]);
    }
  }
}

};   // namespace SimpleComplex






SimpleComplex::TetrahedronMesh SimpleComplex::TetBoolean::GetTetMesh(
  const SimpleComplex::TetBoolean::TwoVecInt& twoVecInt) {
  // should be easy
  // when loading tetMesh from txt
  // 4 triangles of one tet should be consecutive
  // twoVecInt should be like
  // (1)  face :[0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0]
  // (2)  vertices: [...]
}