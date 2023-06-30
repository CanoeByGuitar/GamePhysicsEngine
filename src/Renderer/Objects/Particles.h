//
// Created by 王晨辉 on 2023/6/23.
//

#ifndef GAMEPHYSICSINONEWEEKEND_PARTICLES_H
#define GAMEPHYSICSINONEWEEKEND_PARTICLES_H

#include <Geometry/ParticleSystem.h>
#include <Renderer/GLVertexArray.h>
#include "ObectBase.h"

namespace renderer {
    struct PlainParticleVertex {
        vec3 position;
    };

    class Particles : public Object {

    public:
        Particles(const std::string &name,
                  const std::shared_ptr<geo::Particles3D> &mParticles,
                  DrawMode mode = DrawMode::STATIC,
                  PrimitiveType type = TRIANGLE)
                  : Object(name, mode, type),
                  m_particles(mParticles) {}


        void SetPipelineData() override {
            m_VAO.Init();
            m_VAO.Bind();

            m_VAO.AttachBuffer(BufferType::ARRAY,
                               m_vertices.size() * sizeof(PlainParticleVertex),
                               m_vertices.data(),
                               m_drawMode);

            m_VAO.EnableAttribute(0, 3, sizeof(PlainParticleVertex), (void*)0);

        }

        void SetupVerticesBuffer() override {
            m_vertices.clear();
            int n = (int)m_particles->m_pos.size();
            m_vertices.reserve(n + 1);
            for(int i = 0; i < n; i++){
                m_vertices.push_back({m_particles->m_pos[i]});
            }

            // for drawing line p0 - p1 - p2 - p0
            if(m_primitiveType == PrimitiveType::LINE){
                m_vertices.push_back({m_particles->m_pos[0]});
            }
        }

        void SetMaterial() override {
            PHY_ASSERT(1, "No Particles Material For now!");
        }


        void Draw() override {
            if(m_primitiveType == PrimitiveType::LINE){
                glLineWidth(1.f);
                glDrawArrays(GL_LINE_STRIP, 0, (int)m_particles->m_pos.size() + 1);
            }else{
                glPointSize(5.0f);
                glDrawArrays(GL_POINTS, 0, (int)m_particles->m_pos.size());
            }

        }

    private:
        std::shared_ptr<geo::Particles3D> m_particles;
        std::vector<PlainParticleVertex> m_vertices;
    };
}

#endif //GAMEPHYSICSINONEWEEKEND_PARTICLES_H
