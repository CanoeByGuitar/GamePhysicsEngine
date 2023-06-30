//
// Created by 王晨辉 on 2023/5/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_MESH_H
#define GAMEPHYSICSINONEWEEKEND_MESH_H

#include <Geometry/Geometry.h>
#include <Renderer/GLVertexArray.h>
#include "ObectBase.h"

namespace renderer {
struct MeshVertex {
    vec3 Position;
    vec3 Normal;
    vec2 Coord;
    explicit MeshVertex(const vec3& position) : Position(position) {}
    MeshVertex(const vec3& position, const vec3& normal, const vec2& coord)
        : Position(position), Normal(normal), Coord(coord) {}
};

class Mesh : public Object {
   public:
    Mesh(const char* name, const std::shared_ptr<geo::Mesh>& GeoMesh,
         DrawMode mode = DrawMode::DYNAMIC,
         PrimitiveType type = PrimitiveType::TRIANGLE)
        : Object(name, mode, type), m_geoMesh(GeoMesh) {}

    void SetPipelineData() override {
        m_VAO.Init();
        m_VAO.Bind();

        // vertices
        m_VAO.AttachBuffer(BufferType::ARRAY,
                           m_vertices.size() * sizeof(MeshVertex),
                           m_vertices.data(), m_drawMode);
        // indices
        m_VAO.AttachBuffer(BufferType::ELEMENT,
                           m_indices.size() * sizeof(unsigned int),
                           m_indices.data(), m_drawMode);

        m_VAO.EnableAttribute(0, 3, sizeof(MeshVertex), (void*)0);
        m_VAO.EnableAttribute(1, 3, sizeof(MeshVertex), (void*)3);
        m_VAO.EnableAttribute(2, 2, sizeof(MeshVertex), (void*)6);
    };

    void SetupVerticesBuffer() override {
        auto triangleNum = m_geoMesh->triangles.size();
        m_vertices.clear();
        m_indices.clear();
        m_vertices.reserve(triangleNum * 3);
        m_indices.reserve(triangleNum * 3);

        unsigned int idx = 0;
        for (auto tri : m_geoMesh->triangles) {
            for (int i = 0; i < 3; i++) {
                m_vertices.push_back(MeshVertex{tri->points[i]});
                m_indices.push_back(idx++);
            }
        }
    };

    void SetMaterial() override{

    };

   private:
    std::shared_ptr<geo::Mesh> m_geoMesh;
    std::vector<MeshVertex> m_vertices;
};

}  // namespace renderer

#endif  //GAMEPHYSICSINONEWEEKEND_MESH_H
