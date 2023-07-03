//
// Created by 王晨辉 on 2023/5/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_CUBE_H
#define GAMEPHYSICSINONEWEEKEND_CUBE_H

#include <Geometry/Geometry.h>
#include <Renderer/GLVertexArray.h>
#include "ObectBase.h"

namespace renderer {
struct CubeVertex {
    vec3 position;
};

class Cube : public Object {
    using AABB = geo::AABB;

   public:
    Cube(const char* name, const std::shared_ptr<AABB>& bound,
         DrawMode mode = DrawMode::DYNAMIC,
         PrimitiveType type = PrimitiveType::TRIANGLE,
         MaterialPtr mtl = nullptr)
        : Object(name, mode, type, std::move(mtl)), m_bound(bound) {}

    void SetPipelineData() override {
        m_VAO.Init();
        m_VAO.Bind();

        m_VAO.AttachBuffer(BufferType::ARRAY,
                           m_vertices.size() * sizeof(CubeVertex),
                           m_vertices.data(), m_drawMode);
        m_VAO.AttachBuffer(BufferType::ELEMENT,
                           m_indices.size() * sizeof(unsigned int),
                           m_indices.data(), m_drawMode);

        m_VAO.EnableAttribute(0, 3, sizeof(CubeVertex), (void*)0);
    }

    void SetupVerticesBuffer() override {
        m_vertices.clear();
        m_vertices.reserve(8);
        for (int i : {-1, 1}) {
            for (int j : {-1, 1}) {
                for (int k : {-1, 1}) {
                    auto position = m_bound->position;
                    auto halfSize = m_bound->halfSize;
                    m_vertices.push_back(
                        {{position + vec3(i, j, k) * halfSize}});
                }
            }
        }

        if (m_primitiveType == PrimitiveType::TRIANGLE && m_indices.empty()) {
            m_indices = std::vector<unsigned int>(
                {1, 5, 7, 1, 7, 3, 5, 4, 6, 5, 6, 7, 0, 4, 6, 0, 6, 2,
                 1, 0, 2, 1, 2, 3, 3, 7, 6, 3, 6, 2, 1, 5, 4, 1, 4, 0});
        } else if (m_indices.empty())
            m_indices =
                std::vector<unsigned int>({0, 1, 1, 3, 3, 2, 2, 0, 1, 5, 3, 7,
                                           2, 6, 0, 4, 4, 5, 5, 7, 7, 6, 6, 4});
    }

    void SetMaterial() override { PHY_ASSERT(1, "No Cube Material For now!"); }

    void Draw() override {
        if (m_primitiveType == PrimitiveType::LINE) {
            //                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawElements(GL_LINES, m_indices.size(), GL_UNSIGNED_INT,
                           nullptr);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT,
                           nullptr);
        }
    }

   private:
    std::vector<CubeVertex> m_vertices;
    std::shared_ptr<AABB> m_bound;
};
}  // namespace renderer

#endif  //GAMEPHYSICSINONEWEEKEND_CUBE_H
