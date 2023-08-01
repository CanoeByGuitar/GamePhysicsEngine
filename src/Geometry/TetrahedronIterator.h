//
// Created by 王晨辉 on 2023/7/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_TETRAHEDRONITERATOR_H
#define GAMEPHYSICSINONEWEEKEND_TETRAHEDRONITERATOR_H

#include "TetrahedronMesh.h"

namespace SimpleComplex{


class VertexIterator{
public:
  explicit VertexIterator(const TetrahedronMesh& tetMesh);
  void Advance();
  bool Done();
  VertexPtr Current();


private:
  int m_idx;
  const TetrahedronMesh& m_tetMesh;
};

class EdgeIterator{
public:
  explicit EdgeIterator(const TetrahedronMesh& tetMesh);
  void Advance();
  bool Done();
  EdgePtr Current();


private:
  int m_idx;
  const TetrahedronMesh& m_tetMesh;
};

class FaceIterator{
public:
  explicit FaceIterator(const TetrahedronMesh& tetMesh);
  void Advance();
  bool Done();
  FacePtr Current();


private:
  int m_idx;
  const TetrahedronMesh& m_tetMesh;
};

class TetIterator{
public:
  explicit TetIterator(const TetrahedronMesh& tetMesh);
  void Advance();
  bool Done();
  TetPtr Current();


private:
  int m_idx;
  const TetrahedronMesh& m_tetMesh;
};



}



#endif   // GAMEPHYSICSINONEWEEKEND_TETRAHEDRONITERATOR_H
