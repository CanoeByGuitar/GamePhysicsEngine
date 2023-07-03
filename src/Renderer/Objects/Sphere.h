//
// Created by 王晨辉 on 2023/6/18.
//

#ifndef GAMEPHYSICSINONEWEEKEND_SPHERE_H
#define GAMEPHYSICSINONEWEEKEND_SPHERE_H

#include <Geometry/Geometry.h>
#include <Renderer/GLVertexArray.h>
#include "ObectBase.h"

namespace renderer {
struct SphereVertex {
    vec3 position;
};

class Sphere : public Object {
    using sphere = geo::Sphere;

   public:
    Sphere(const char* name, const std::shared_ptr<sphere>& _sphere,
           DrawMode mode = DrawMode::DYNAMIC,
           PrimitiveType type = PrimitiveType::TRIANGLE,
           MaterialPtr mtl = nullptr)
        : Object(name, mode, type, std::move(mtl)), m_sphere(_sphere) {}

    void SetPipelineData() override {
        m_VAO.Init();
        m_VAO.Bind();

        m_VAO.AttachBuffer(BufferType::ARRAY,
                           m_vertices.size() * sizeof(SphereVertex),
                           m_vertices.data(), m_drawMode);
        m_VAO.AttachBuffer(BufferType::ELEMENT,
                           m_indices.size() * sizeof(unsigned int),
                           m_indices.data(), m_drawMode);

        m_VAO.EnableAttribute(0, 3, sizeof(SphereVertex), (void*)0);
    }

    void SetupVerticesBuffer() override {
        const int slices = 20;  // 经度切片数
        const int stacks = 20;  // 纬度切片数

        // 生成球体的顶点数据
        m_vertices.clear();
        m_vertices.reserve(slices * stacks);
        for (int i = 0; i <= stacks; ++i) {
            float latitude =
                glm::pi<float>() * (-0.5f + static_cast<float>(i) / stacks);
            float sinLat = std::sin(latitude);
            float cosLat = std::cos(latitude);

            for (int j = 0; j <= slices; ++j) {
                float longitude =
                    2 * glm::pi<float>() * static_cast<float>(j) / slices;
                float sinLong = std::sin(longitude);
                float cosLong = std::cos(longitude);

                float x = m_sphere->radius * cosLong * cosLat;
                float y = m_sphere->radius * sinLat;
                float z = m_sphere->radius * sinLong * cosLat;

                m_vertices.push_back({vec3(x, y, z) + m_sphere->position});
            }
        }

        // 生成球体的索引数据
        m_indices.clear();
        m_indices.reserve(slices * stacks);
        for (int i = 0; i < stacks; ++i) {
            int k1 = i * (slices + 1);
            int k2 = k1 + slices + 1;

            for (int j = 0; j < slices; ++j, ++k1, ++k2) {
                if (i != 0) {
                    m_indices.push_back(k1);
                    m_indices.push_back(k2);
                    m_indices.push_back(k1 + 1);
                }

                if (i != (stacks - 1)) {
                    m_indices.push_back(k1 + 1);
                    m_indices.push_back(k2);
                    m_indices.push_back(k2 + 1);
                }
            }
        }
    }

    void SetMaterial() override {
        PHY_ASSERT(1, "No Sphere Material For now!");
    }

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
    GeoSpherePtr m_sphere;
    std::vector<SphereVertex> m_vertices;
};
}  // namespace renderer

#endif  //GAMEPHYSICSINONEWEEKEND_SPHERE_H
