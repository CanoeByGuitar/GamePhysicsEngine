//
// Created by 王晨辉 on 2023/7/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_TETRAHEDRONITERATOR_H
#define GAMEPHYSICSINONEWEEKEND_TETRAHEDRONITERATOR_H

#include "TetrahedronMesh.h"

namespace SimpleComplex {


class VertexIterator {
public:
  explicit VertexIterator(TetrahedronMesh& mesh);
  void    Advance();
  bool    Done();
  Vertex& Current();
  int     Idx() const { return m_idx; }

private:
  int              m_idx;
  TetrahedronMesh& m_tetMesh;
};

class EdgeIterator {
public:
  explicit EdgeIterator(TetrahedronMesh& mesh);
  void  Advance();
  bool  Done();
  Edge& Current();


private:
  int              m_idx;
  TetrahedronMesh& m_tetMesh;
};

class FaceIterator {
public:
  explicit FaceIterator(TetrahedronMesh& mesh);
  void  Advance();
  bool  Done();
  Face& Current();


private:
  int              m_idx;
  TetrahedronMesh& m_tetMesh;
};

class TetIterator {
public:
  explicit TetIterator(TetrahedronMesh& mesh);
  void Advance();
  bool Done();
  Tet& Current();


private:
  int              m_idx;
  TetrahedronMesh& m_tetMesh;
};



}   // namespace SimpleComplex



#endif   // GAMEPHYSICSINONEWEEKEND_TETRAHEDRONITERATOR_H
