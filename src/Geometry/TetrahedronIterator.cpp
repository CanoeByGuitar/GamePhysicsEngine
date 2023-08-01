//
// Created by 王晨辉 on 2023/7/28.
//

#include "TetrahedronIterator.h"

namespace SimpleComplex {
////////// VertexIterator
VertexIterator::VertexIterator(const TetrahedronMesh& tetMesh)
  : m_idx(0)
  , m_tetMesh(tetMesh) {}


void VertexIterator::Advance() {
  m_idx++;
}

bool VertexIterator::Done() {
  return m_idx >= m_tetMesh.m_vertices.size();
}


VertexPtr VertexIterator::Current() {
  return m_tetMesh.m_vertices[m_idx];
}


////////// EdgeIterator
EdgeIterator::EdgeIterator(const TetrahedronMesh& tetMesh)
  : m_idx(0)
  , m_tetMesh(tetMesh) {}


void EdgeIterator::Advance() {
  m_idx++;
}

bool EdgeIterator::Done() {
  return m_idx >= m_tetMesh.m_edges.size();
}


EdgePtr EdgeIterator::Current() {
  return m_tetMesh.m_edges[m_idx];
}


////////// FaceIterator
FaceIterator::FaceIterator(const TetrahedronMesh& tetMesh)
  : m_idx(0)
  , m_tetMesh(tetMesh) {}


void FaceIterator::Advance() {
  m_idx++;
}

bool FaceIterator::Done() {
  return m_idx >= m_tetMesh.m_faces.size();
}


FacePtr FaceIterator::Current() {
  return m_tetMesh.m_faces[m_idx];
}



////////// TetIterator
TetIterator::TetIterator(const TetrahedronMesh& tetMesh)
  : m_idx(0)
  , m_tetMesh(tetMesh) {}


void TetIterator::Advance() {
  m_idx++;
}

bool TetIterator::Done() {
  return m_idx >= m_tetMesh.m_tets.size();
}


TetPtr TetIterator::Current() {
  return m_tetMesh.m_tets[m_idx];
}



}   // namespace SimpleComplex
