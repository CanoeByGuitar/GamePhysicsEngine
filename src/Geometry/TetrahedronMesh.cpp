//
// Created by 王晨辉 on 2023/7/27.
//

#include "TetrahedronMesh.h"

SimpleComplex::Vertex::Vertex(float x, float y, float z)
  : x(x)
  , y(y)
  , z(z) {}

vec3 SimpleComplex::Vertex::ToVec3() const {
  return {x, y, z};
}

SimpleComplex::Edge::Edge(const std::shared_ptr<Vertex>& a, const std::shared_ptr<Vertex>& b) {
  m_verts[0] = a;
  m_verts[1] = b;
}

vec3 SimpleComplex::Edge::GetDir() {
  return {m_verts[1]->ToVec3() - m_verts[0]->ToVec3()};
}



SimpleComplex::Face::Face(const std::vector<std::shared_ptr<Vertex>>& verts) {
  auto e0 = std::make_shared<Edge>(verts[0], verts[1]);
  auto e1 = std::make_shared<Edge>(verts[1], verts[2]);
  auto e2 = std::make_shared<Edge>(verts[2], verts[0]);
  for (int i = 0; i < 3; i++) {
    m_verts[i] = verts[i];
  }
  m_edges[0] = e0;
  m_edges[1] = e1;
  m_edges[2] = e2;
  m_normal   = glm::normalize(glm::cross(m_edges[0]->GetDir(), m_edges[1]->GetDir()));
}




SimpleComplex::Tet::Tet(const std::vector<std::shared_ptr<Vertex>>& verts) {
  //            0  \
  //          /   \   3
  //         /     \ /
  //        1 ------2
  auto f0 =
    std::make_shared<Face>(std::vector<std::shared_ptr<Vertex>>{verts[0], verts[1], verts[2]});
  auto f1 =
    std::make_shared<Face>(std::vector<std::shared_ptr<Vertex>>{verts[0], verts[2], verts[3]});
  auto f2 =
    std::make_shared<Face>(std::vector<std::shared_ptr<Vertex>>{verts[0], verts[3], verts[1]});
  auto f3 =
    std::make_shared<Face>(std::vector<std::shared_ptr<Vertex>>{verts[1], verts[3], verts[2]});

  m_faces[0] = f0;
  m_faces[1] = f1;
  m_faces[2] = f2;
  m_faces[3] = f3;

  m_edges[0] = f0->m_edges[0];
  m_edges[1] = f0->m_edges[1];
  m_edges[2] = f0->m_edges[2];
  m_edges[3] = f1->m_edges[1];
  m_edges[4] = f1->m_edges[2];
  m_edges[5] = f2->m_edges[2];

  m_verts[0] = f0->m_verts[0];
  m_verts[1] = f0->m_verts[1];
  m_verts[2] = f0->m_verts[2];
  m_verts[3] = f0->m_verts[2];
}

SimpleComplex::TetrahedronMesh::TetrahedronMesh(std::vector<Vertex>&& vertices,
                                                std::vector<int>&&    indices) {
  m_vertices.reserve(vertices.size());
  m_tets.reserve(indices.size());
  for (const auto& v : vertices) {
    m_vertices.push_back(std::make_shared<Vertex>(v));
  }


  m_edges.clear();
  m_faces.clear();
  for (int i = 0; i < indices.size(); i += 4) {
    auto tet =
      std::make_shared<Tet>(std::vector<std::shared_ptr<Vertex>>{m_vertices[indices[i]],
                                                                 m_vertices[indices[i + 1]],
                                                                 m_vertices[indices[i + 2]],
                                                                 m_vertices[indices[i + 3]]});
    m_tets.push_back(tet);
    for (const auto& f : tet->m_faces) {
      m_faces.push_back(f);
    }
    for (const auto& e : tet->m_edges) {
      m_edges.push_back(e);
    }
  }

}


GeoMeshPtr SimpleComplex::TetrahedronMesh::ToGeoMesh() const {
  auto mesh = std::make_shared<geo::Mesh>();
  int  cnt  = 0;
  for (const auto& face : m_faces) {
    for (int i = 0; i < 3; i++) {
      mesh->vertices.push_back(face->m_verts[i]->ToVec3());
      mesh->indices.push_back(cnt);
      cnt++;

      PHY_DEBUG("Vertex {} : {}", cnt, face->m_verts[i]->ToVec3());
    }
  }

  return mesh;
}
