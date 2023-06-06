//
// Created by 王晨辉 on 2023/5/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_MESH_H
#define GAMEPHYSICSINONEWEEKEND_MESH_H

#include <Geometry/Geometry.h>
#include <Renderer/GLVertexArray.h>
#include "ObectBase.h"

namespace renderer{
    struct BasicMeshVertex{
        vec3 position;
    };

    class BasicMesh : public ObjectBase<BasicMeshVertex>{
    public:
        BasicMesh(const char* name,
                  const std::shared_ptr<geo::Mesh> &GeoMesh,
                  DrawMode mode = DrawMode::DYNAMIC,
                  PrimitiveType type = PrimitiveType::TRIANGLE,
                  vec3 color = vec3(-1))
        : ObjectBase(name, mode, type, color), m_GeoMesh(GeoMesh){

        }
        void SetPipelineData() override{
            m_VAO.Init();
            m_VAO.Bind();

            // vertices
            m_VAO.AttachBuffer(BufferType::ARRAY,
                               m_vertices.size() * sizeof(BasicMeshVertex),
                               m_vertices.data(),
                               m_drawMode);
            // indices
            m_VAO.AttachBuffer(BufferType::ELEMENT,
                               m_indices.size() * sizeof(unsigned int),
                               m_indices.data(),
                               m_drawMode);

            m_VAO.EnableAttribute(0, 3, sizeof(BasicMeshVertex), (void*)0);
        };

        void SetupVerticesBuffer() override{
            auto triangleNum = m_GeoMesh->triangles.size();
            m_vertices.clear();
            m_indices.clear();
            m_vertices.reserve(triangleNum * 3);
            m_indices.reserve(triangleNum * 3);

            unsigned int idx = 0;
            for(auto tri : m_GeoMesh->triangles){
                for(int i = 0; i < 3; i++){
                    m_vertices.push_back({tri->points[i]});
                    m_indices.push_back(idx++);
                }
            }
        };

        void SetMaterial() override{
            PHY_ASSERT(1, "No BasicMesh Material For now!");
        };


    private:
        std::shared_ptr<geo::Mesh> m_GeoMesh;
    };

    class BasicModel : public ObjectBase<BasicMeshVertex>{
    public:
        BasicModel(const char* name,
                  const std::shared_ptr<geo::Model> &model,
                  DrawMode mode = DrawMode::DYNAMIC,
                  PrimitiveType type = PrimitiveType::TRIANGLE,
                   vec3 color = vec3(-1))
                : ObjectBase(name, mode, type, color), m_GeoModel(model){

        }
        void SetPipelineData() override{
            m_VAO.Init();
            m_VAO.Bind();

            // vertices
            m_VAO.AttachBuffer(BufferType::ARRAY,
                               m_vertices.size() * sizeof(BasicMeshVertex),
                               m_vertices.data(),
                               m_drawMode);
            // indices
            m_VAO.AttachBuffer(BufferType::ELEMENT,
                               m_indices.size() * sizeof(unsigned int),
                               m_indices.data(),
                               m_drawMode);

            m_VAO.EnableAttribute(0, 3, sizeof(BasicMeshVertex), (void*)0);
        };

        void SetupVerticesBuffer() override{
            m_vertices.clear();
            m_indices.clear();
            unsigned int idx = 0;
            for(const auto& mesh : m_GeoModel->m_meshes){
                auto triangleNum = mesh.triangles.size();
                m_vertices.reserve(m_vertices.size() + triangleNum * 3);
                m_indices.reserve(m_indices.size() + triangleNum * 3);
                for(auto tri : mesh.triangles){
                    for(int i = 0; i < 3; i++){
                        m_vertices.push_back({tri->points[i]});
                        m_indices.push_back(idx++);
                    }
                }
            }

        };

        void SetMaterial() override{
            PHY_ASSERT(1, "No BasicMesh Material For now!");
        };


    private:
        std::shared_ptr<geo::Model> m_GeoModel;
    };
}

#endif //GAMEPHYSICSINONEWEEKEND_MESH_H
