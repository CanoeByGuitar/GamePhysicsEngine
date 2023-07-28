//
// Created by 王晨辉 on 2023/7/27.
//

#ifndef GAMEPHYSICSINONEWEEKEND_TETLOADER_H
#define GAMEPHYSICSINONEWEEKEND_TETLOADER_H

#include <Base/Log.h>
#include <Geometry/TetrahedronMesh.h>
#include <fstream>
#include <iostream>
#include <sstream>

class TetLoader {
  using Vertex         = SimpleComplex::Vertex;
  using TetrahedroMesh = SimpleComplex::TetrahedronMesh;

public:
  static std::unique_ptr<TetrahedroMesh> Load(std::string&& path) {
    std::ifstream       file(path);
    std::string         line;
    bool                readingVertices    = false;
    bool                readingTetrahedron = false;
    std::vector<Vertex> vertices;
    std::vector<int>    indices;
    while (std::getline(file, line)) {
      if(line.empty()) continue;
      if (line == "vert") {
        readingVertices = true;
        continue;
      }

      if (line == "tet") {
        readingTetrahedron = true;
        readingVertices = false;
        continue;
      }

      if (readingVertices) {
        std::istringstream iss(line);
        float              x, y, z;
        iss >> x >> y >> z;
        Vertex v{x, y, z};
        vertices.push_back(v);
      } else if (readingTetrahedron) {
        std::istringstream iss(line);
        std::vector<int>   v(4);
        iss >> v[0] >> v[1] >> v[2] >> v[3];
        indices.insert(indices.end(), v.begin(), v.end());
      }
    }
    auto tetMesh = std::make_unique<TetrahedroMesh>(std::move(vertices), std::move(indices));
    return tetMesh;
  }
};

#endif   // GAMEPHYSICSINONEWEEKEND_TETLOADER_H
