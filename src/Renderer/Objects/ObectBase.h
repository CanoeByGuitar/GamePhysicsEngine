//
// Created by 王晨辉 on 2023/5/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_OBECTBASE_H
#define GAMEPHYSICSINONEWEEKEND_OBECTBASE_H

#include <Base/IOUtils.h>
#include <Renderer/GLVertexArray.h>
#include <Renderer/Shader.h>
#include <utility>
#include <vector>

namespace renderer {
    class Object {
    public:
        explicit Object(std::string name,
                        DrawMode mode = DrawMode::DYNAMIC,
                        PrimitiveType type = PrimitiveType::TRIANGLE)
            : m_name(std::move(name)), m_drawMode(mode), m_primitiveType(type) {
            m_VAO = GLVertexArray();
        }
        virtual ~Object() = default;
        virtual void SetPipelineData() = 0;
        virtual void SetupVerticesBuffer() = 0;
        virtual void SetMaterial() = 0;
        virtual void BindVAO() { m_VAO.Bind(); }
        virtual void UnBindVAO() { m_VAO.UnBind(); };
        virtual void Draw() {
            if (m_primitiveType == PrimitiveType::LINE) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            glDrawElements(GL_TRIANGLES, (int)m_indices.size(), GL_UNSIGNED_INT, nullptr);
        };

    public:
        bool m_isVisible = true;
        std::string m_name;
        DrawMode m_drawMode;
        PrimitiveType m_primitiveType;
        std::vector<unsigned int> m_indices;
        GLVertexArray m_VAO{};
    };

}// namespace renderer

using RenderObjPtr = std::shared_ptr<renderer::Object>;

#endif//GAMEPHYSICSINONEWEEKEND_OBECTBASE_H
