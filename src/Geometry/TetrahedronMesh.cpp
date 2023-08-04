//
// Created by 王晨辉 on 2023/7/27.
//

#include "TetrahedronMesh.h"
#include "unordered_map"
#include <algorithm>
#include <glm/gtc/epsilon.hpp>

SimpleComplex::Vertex::Vertex(int idx, float x, float y, float z)
  : m_idx(idx)
  , x(x)
  , y(y)
  , z(z) {}

vec3 SimpleComplex::Vertex::ToVec3() const {
  return {x, y, z};
}



SimpleComplex::Edge::Edge(int idx, std::vector<int>&& verts)
  : m_idx(idx)
  , m_verts(verts) {}


bool SimpleComplex::Edge::operator==(const SimpleComplex::Edge& other) const {
  auto& a = this->m_verts;
  auto& b = other.m_verts;
  return (a[0] == b[0] && a[1] == b[1]) || (a[0] == b[1] && a[1] == b[0]);
}

SimpleComplex::Face::Face(int idx, std::vector<int>&& verts)
  : m_idx(idx)
  , m_verts(verts) {}



bool SimpleComplex::Face::operator==(const SimpleComplex::Face& other) const {
  std::vector temp1 = this->m_verts;
  std::vector temp2 = other.m_verts;
  std::sort(temp1.begin(), temp1.end());
  std::sort(temp2.begin(), temp2.end());
  return temp1[0] == temp2[0] && temp1[1] == temp2[1] && temp1[2] == temp2[2];
}

SimpleComplex::Tet::Tet(int idx, std::vector<int>&& verts, std::vector<int>&& edges,
                        std::vector<int>&& faces)
  : m_idx(idx)
  , m_verts(verts)
  , m_edges(edges)
  , m_faces(faces) {}



SimpleComplex::TetrahedronMesh::TetrahedronMesh(std::vector<Vertex>&& vertices,
                                                std::vector<int>&&    indices) {
  CreateVertexList(std::move(vertices));
  CreateTetList(std::move(indices));
}


GeoMeshPtr SimpleComplex::TetrahedronMesh::ToGeoMesh() const {
  auto mesh = std::make_shared<geo::Mesh>();
  int  cnt  = 0;
  for (const auto& face : m_faces) {
    for (int i = 0; i < 3; i++) {
      mesh->vertices.push_back(m_vertices[face.m_verts[i]].ToVec3());
      mesh->indices.push_back(cnt);
      cnt++;
    }
  }
  return mesh;
}

void SimpleComplex::TetrahedronMesh::CreateVertexList(std::vector<Vertex>&& vertices) {
  m_vertices.reserve(vertices.size());
  int cnt = 0;
  for (auto& vertex : vertices) {
    m_vertices.emplace_back(cnt, vertex.x, vertex.y, vertex.z);
    cnt++;
  }
  m_VV.resize(vertices.size());
  m_VF.resize(vertices.size());
}




void SimpleComplex::TetrahedronMesh::CreateTetList(std::vector<int>&& indices) {
  m_edges.reserve(indices.size() / 4 * 6);
  m_faces.reserve(indices.size());
  m_tets.reserve(indices.size() / 4);
  for (int i = 0; i < indices.size(); i += 4) {
    AddTet(indices[i], indices[i + 1], indices[i + 2], indices[i + 3]);
  }
  m_edges.shrink_to_fit();
  m_faces.shrink_to_fit();
  m_tets.shrink_to_fit();
}




int SimpleComplex::TetrahedronMesh::AddEdge(int a, int b) {
  int  edge_cnt = static_cast<int>(m_edges.size());
  Edge e{edge_cnt, {a, b}};
  if (m_edgeMap.find(e) == m_edgeMap.end()) {
    m_edgeMap[e] = edge_cnt;
    m_VV[a].push_back(b);
    m_VV[b].push_back(a);
    m_edges.push_back(e);
    return edge_cnt;
  } else {
    return m_edgeMap[e];
  }
}




int SimpleComplex::TetrahedronMesh::AddFace(int a, int b, int c) {
  int  face_cnt = static_cast<int>(m_faces.size());
  Face f(face_cnt, {a, b, c});
  if (m_faceMap.find(f) == m_faceMap.end()) {
    m_faceMap[f] = face_cnt;
    m_VF[a].push_back(face_cnt);
    m_VF[b].push_back(face_cnt);
    m_VF[c].push_back(face_cnt);
    f.m_normal = glm::cross(m_vertices[b].ToVec3() - m_vertices[a].ToVec3(),
                            m_vertices[c].ToVec3() - m_vertices[b].ToVec3());
    m_faces.push_back(f);
    return face_cnt;
  } else {
    return m_faceMap[f];
  }
}

int SimpleComplex::TetrahedronMesh::AddTet(int a, int b, int c, int d) {
  // add edges
  auto e0 = AddEdge(a, b);
  auto e1 = AddEdge(a, c);
  auto e2 = AddEdge(a, d);
  auto e3 = AddEdge(b, c);
  auto e4 = AddEdge(c, d);
  auto e5 = AddEdge(d, b);

  // add faces
  auto f0 = AddFace(a, b, c);
  auto f1 = AddFace(a, c, d);
  auto f2 = AddFace(a, d, b);
  auto f3 = AddFace(b, d, c);

  // add tet
  int tet_cnt = static_cast<int>(m_tets.size());
  m_tets.push_back({tet_cnt, {a, b, c, d}, {e0, e1, e2, e3, e4, e5}, {f0, f1, f2, f3}});
  return tet_cnt;
}

int SimpleComplex::TetrahedronMesh::AddVertex(const vec3& v) {
  int vertex_cnt = (int)m_vertices.size();
  m_vertices.emplace_back(vertex_cnt, v.x, v.y, v.z);

  // enlarge m_VV and m_VF
  // todo: accelerate
  m_VV.resize(m_VV.size() + 1);
  m_VF.resize(m_VF.size() + 1);
  return vertex_cnt;
}

SimpleComplex::Intersection SimpleComplex::GetIntersection(const vec3& o, const vec3& end,
                                                           const vec3& p0, const vec3& p1,
                                                           const vec3& p2) {

  auto d  = glm::normalize(end - o);
  auto e1 = p1 - p0;
  auto e2 = p2 - p0;
  auto s  = o - p0;
  auto s1 = glm::cross(d, e2);
  auto s2 = glm::cross(s, e1);

  auto  s1e1 = glm::dot(s1, e1);
  float t    = glm::dot(s2, e2) / s1e1;
  float b1   = glm::dot(s1, s) / s1e1;
  float b2   = glm::dot(s2, d) / s1e1;

  bool inEdge = false;
  if (t <= glm::distance(o, end))
    inEdge = true;
  Intersection inter;
  if (inEdge && b1 >= 0 && b1 <= 1 && b2 >= 0 && b2 <= 1 && b1 + b2 <= 1) {
    inter.m_isHit    = true;
    inter.m_hitPoint = o + t * d;
  }

  float epsilon = 1e-3;
  if (glm::all(glm::epsilonEqual(inter.m_hitPoint, p0, epsilon)) ||
      glm::all(glm::epsilonEqual(inter.m_hitPoint, p1, epsilon)) ||
      glm::all(glm::epsilonEqual(inter.m_hitPoint, p2, epsilon))) {
    inter.m_isHit = false;
    inter.m_hitPoint = vec3{0};
  }
  //  if (t <= glm::distance(o, end))  inEdge = true;
  //  Intersection inter;
  //  if (inEdge && b1 > 0 && b1 < 1 && b2 > 0 && b2 < 1 && b1 + b2 < 1){
  //    inter.m_isHit = true;
  //    inter.m_hitPoint = o + t * d;
  //  }
  return inter;
}
