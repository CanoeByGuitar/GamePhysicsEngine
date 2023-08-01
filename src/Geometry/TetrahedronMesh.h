//
// Created by 王晨辉 on 2023/7/27.
//

#ifndef GAMEPHYSICSINONEWEEKEND_TETRAHEDRONMESH_H
#define GAMEPHYSICSINONEWEEKEND_TETRAHEDRONMESH_H

#include "Geometry.h"
#include <Base/Log.h>
#include <unordered_map>
#include <vector>

namespace SimpleComplex {

struct Vertex {
  int   m_idx;
  float x;
  float y;
  float z;
  Vertex(int idx, float x, float y, float z);
  vec3 ToVec3() const;
};


struct Edge {
  int              m_idx;
  std::vector<int> m_verts;   // size = 2
  Edge(int idx, std::vector<int>&& verts);

  bool operator==(const Edge& other) const;   // for hash
};

struct Face {
  int              m_idx;
  std::vector<int> m_verts;   // size = 3
  vec3             m_normal;
  Face(int idx, std::vector<int>&& verts);

  bool operator==(const Face& other) const;   // for hash
};

struct Tet {
  int              m_idx;
  std::vector<int> m_verts;   // size = 4
  std::vector<int> m_edges;   // size = 6
  std::vector<int> m_faces;   // size = 4
  Tet(int idx, std::vector<int>&& verts, std::vector<int>&& edges, std::vector<int>&& faces);
};

struct EdgeHash {
  size_t operator()(const Edge& e) const {
    std::size_t x_hash = std::hash<int>{}(e.m_verts[0]);
    std::size_t y_hash = std::hash<int>{}(e.m_verts[1]);
    return x_hash ^ (y_hash << 1);
  }
};

struct FaceHash {
  size_t operator()(const Face& f) const {
    std::size_t x_hash = std::hash<int>{}(f.m_verts[0]);
    std::size_t y_hash = std::hash<int>{}(f.m_verts[1]);
    std::size_t z_hash = std::hash<int>{}(f.m_verts[2]);
    return x_hash ^ (y_hash << 1) ^ (z_hash << 2);
  }
};

class TetrahedronMesh {
public:
  TetrahedronMesh() = default;
  TetrahedronMesh(std::vector<Vertex>&& vertices, std::vector<int>&& indices);
  GeoMeshPtr ToGeoMesh() const;
  void       CreateVertexList(std::vector<Vertex>&& vertices);   // vertices => m_vertices
  void CreateTetList(std::vector<int>&& indices);   // indices, m_vertices => all data structure
  int  AddEdge(int a, int b);
  int  AddFace(int a, int b, int c);
  int  AddTet(int a, int b, int c, int d);

  int AddVertex(const vec3& v);
  std::vector<Vertex> m_vertices;
  std::vector<Edge>   m_edges;
  std::vector<Face>   m_faces;
  std::vector<Tet>    m_tets;

  std::vector<std::vector<int>> m_VF;   // vertex to face
  std::vector<std::vector<int>> m_VV;   // vertex to different vertex


  std::unordered_map<Edge, int, EdgeHash> m_edgeMap;   // used to deduplicate
  std::unordered_map<Face, int, FaceHash> m_faceMap;   // used to deduplicate

  //  std::vector<std::vector<int>> m_EE;   // edge to same edge
  //  std::vector<std::vector<int>> m_FF;   // face to same face
};

}   // namespace SimpleComplex



#endif   // GAMEPHYSICSINONEWEEKEND_TETRAHEDRONMESH_H
