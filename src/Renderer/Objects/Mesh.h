//
// Created by 王晨辉 on 2023/5/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_MESH_H
#define GAMEPHYSICSINONEWEEKEND_MESH_H

#include <Geometry/Geometry.h>
#include <Renderer/GLVertexArray.h>

#include <utility>
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
    Mesh(const char* name, GeoMeshPtr  GeoMesh,
         DrawMode mode = DrawMode::DYNAMIC,
         PrimitiveType type = PrimitiveType::TRIANGLE,
         MaterialPtr  mtl = std::make_shared<Material>())
        : Object(name, mode, type, std::move(mtl)), m_geoMesh(std::move(GeoMesh)) {}

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
        m_VAO.EnableAttribute(1, 3, sizeof(MeshVertex), (void*)(3 * sizeof(float)));
        m_VAO.EnableAttribute(2, 2, sizeof(MeshVertex), (void*)(6 * sizeof(float)));
    };

    void SetupVerticesBuffer() override {
        m_vertices.clear();
        m_vertices.reserve(m_geoMesh->vertices.size());
        m_indices.clear();
        m_indices.reserve(m_geoMesh->indices.size());
        for(int i = 0; i < m_geoMesh->vertices.size(); i++){
            vec3 normal = (i < m_normals.size()) ? m_normals[i] : vec3(0);
            vec2 coord = (i < m_coords.size()) ? m_coords[i] : vec2(0);
//            PHY_INFO("coord {} : {}", i, m_coords[i]);
            m_vertices.emplace_back(
                m_geoMesh->vertices[i],
                normal,
                coord
                );
        }
        m_indices = m_geoMesh->indices;
    };

    void SetMaterial() override{

    };

   private:
    std::shared_ptr<geo::Mesh> m_geoMesh;
    std::vector<MeshVertex> m_vertices;
};

}  // namespace renderer

#endif  //GAMEPHYSICSINONEWEEKEND_MESH_H
