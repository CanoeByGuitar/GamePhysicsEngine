//
// Created by 王晨辉 on 2023/7/27.
//

#ifndef GAMEPHYSICSINONEWEEKEND_TETRAHEDRONMESH_H
#define GAMEPHYSICSINONEWEEKEND_TETRAHEDRONMESH_H

#include <Base/Log.h>
#include <vector>
#include "Geometry.h"

namespace SimpleComplex {

struct Vertex {
  float x;
  float y;
  float z;
  Vertex(float x, float y, float z);
  vec3 ToVec3() const;
};

struct Edge {
  std::shared_ptr<Vertex> m_verts[2];
  Edge(const std::shared_ptr<Vertex>& a, const std::shared_ptr<Vertex>& b);
  vec3 GetDir();
};

struct Face {
  std::shared_ptr<Vertex> m_verts[3];
  std::shared_ptr<Edge>   m_edges[3];
  vec3                    m_normal;
  Face(const std::vector<std::shared_ptr<Vertex>>& verts);
};

struct Tet {
  std::shared_ptr<Vertex> m_verts[4];
  std::shared_ptr<Edge>   m_edges[6];
  std::shared_ptr<Face>   m_faces[4];
  Tet(const std::vector<std::shared_ptr<Vertex>>& verts);
};

using VertexList = std::vector<std::shared_ptr<Vertex>>;
using EdgeList   = std::vector<std::shared_ptr<Edge>>;
using FaceList   = std::vector<std::shared_ptr<Face>>;
using TetList    = std::vector<std::shared_ptr<Tet>>;
using VertexPtr = std::shared_ptr<Vertex>;
using EdgePtr = std::shared_ptr<Edge>;
using FacePtr = std::shared_ptr<Face>;
using TetPtr = std::shared_ptr<Tet>;


class TetrahedronMesh {
public:
  TetrahedronMesh(std::vector<Vertex>&& vertices,  std::vector<int>&& indices);
  GeoMeshPtr ToGeoMesh() const;

public:
  VertexList m_vertices;
  EdgeList m_edges;
  FaceList m_faces;
  TetList m_tets;

  std::vector<std::vector<int>> m_VF; // vertex to face
  std::vector<std::vector<int>> m_VV; // vertex to vertex

};

}   // namespace SimpleComplex



#endif   // GAMEPHYSICSINONEWEEKEND_TETRAHEDRONMESH_H
