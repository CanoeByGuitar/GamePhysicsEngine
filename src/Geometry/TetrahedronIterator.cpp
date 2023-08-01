//
// Created by 王晨辉 on 2023/7/28.
//

#include "TetrahedronIterator.h"

namespace SimpleComplex {
////////// VertexIterator
VertexIterator::VertexIterator(TetrahedronMesh& mesh)
  : m_idx(0)
  , m_tetMesh(mesh) {}


void VertexIterator::Advance() {
  m_idx++;
}

bool VertexIterator::Done() {
  return m_idx >= m_tetMesh.m_vertices.size();
}


Vertex& VertexIterator::Current(){
  return m_tetMesh.m_vertices[m_idx];
}


////////// EdgeIterator
EdgeIterator::EdgeIterator(TetrahedronMesh& mesh)
  : m_idx(0)
  , m_tetMesh(mesh) {}


void EdgeIterator::Advance() {
  m_idx++;
}

bool EdgeIterator::Done() {
  return m_idx >= m_tetMesh.m_edges.size();
}


Edge& EdgeIterator::Current() {
  return m_tetMesh.m_edges[m_idx];
}


////////// FaceIterator
FaceIterator::FaceIterator(TetrahedronMesh& mesh)
  : m_idx(0)
  , m_tetMesh(mesh) {}


void FaceIterator::Advance() {
  m_idx++;
}

bool FaceIterator::Done() {
  return m_idx >= m_tetMesh.m_faces.size();
}


Face& FaceIterator::Current() {
  return m_tetMesh.m_faces[m_idx];
}



////////// TetIterator
TetIterator::TetIterator(TetrahedronMesh& mesh)
  : m_idx(0)
  , m_tetMesh(mesh) {}


void TetIterator::Advance() {
  m_idx++;
}

bool TetIterator::Done() {
  return m_idx >= m_tetMesh.m_tets.size();
}


Tet& TetIterator::Current() {
  return m_tetMesh.m_tets[m_idx];
}



}   // namespace SimpleComplex
