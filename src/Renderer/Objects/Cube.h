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
        vec3 color = {0, 0, 0};
    };

    class Cube : public ObjectBase<CubeVertex> {
        using AABB = geo::AABB;

    public:
        Cube(const char* name, const AABB &bound, DrawMode mode = DrawMode::DYNAMIC)
                : ObjectBase(name, mode), m_bound(bound) {}

        void SetPipelineData() override {
            m_VAO.Init();
            m_VAO.Bind();

            m_VAO.AttachBuffer(BufferType::ARRAY,
                               m_vertices.size() * sizeof(CubeVertex),
                               m_vertices.data(),
                               m_drawMode);
            m_VAO.AttachBuffer(BufferType::ELEMENT,
                               m_indices.size() * sizeof(unsigned int),
                               m_indices.data(),
                               m_drawMode);

            m_VAO.EnableAttribute(0, 3, sizeof(CubeVertex), (void*)0);

        }

        void SetupVerticesBuffer() override {
            m_vertices.reserve(8);
            for (int i = -1; i <= 1; i += 2) {
                for (int j = -1; j <= 1; j += 2) {
                    for (int k = -1; k <= 1; k += 2) {
                        auto position = m_bound.position;
                        auto halfSize = m_bound.halfSize;
                        m_vertices.push_back({{position + vec3(i, j, k) * halfSize}});
                    }
                }
            }

            m_indices = std::vector<unsigned int>({
                                                          1, 5, 7,
                                                          1, 7, 3,
                                                          5, 4, 6,
                                                          5, 6, 7,
                                                          0, 4, 6,
                                                          0, 6, 2,
                                                          1, 0, 2,
                                                          1, 2, 3,
                                                          3, 7, 6,
                                                          3, 6, 2,
                                                          1, 5, 4,
                                                          1, 4, 0});
        }

        void SetMaterial() override {
            PHY_ASSERT(1, "No Cube Material For now!");
        }

    private:
        AABB m_bound;
    };
}

#endif //GAMEPHYSICSINONEWEEKEND_CUBE_H
