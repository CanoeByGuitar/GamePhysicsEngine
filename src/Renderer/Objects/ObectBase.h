//
// Created by 王晨辉 on 2023/5/28.
//

#ifndef GAMEPHYSICSINONEWEEKEND_OBECTBASE_H
#define GAMEPHYSICSINONEWEEKEND_OBECTBASE_H

#include <Renderer/GLVertexArray.h>
#include <vector>
#include <Renderer/Shader.h>
#include <Base/IOUtils.h>

namespace renderer {
    class Object{
    public:
        explicit Object(const std::string& name):m_name(name){}
        virtual ~Object() = default;
        virtual void SetPipelineData() = 0;
        virtual void SetupVerticesBuffer() = 0;
        virtual void SetMaterial() = 0;
        virtual void BindVAO() = 0;
        virtual void Draw() = 0;

        std::string GetObjName(){
            return m_name;
        }
    private:
        std::string m_name;
    };

    template<typename VertexType>
    class ObjectBase : public Object {
    public:
        ObjectBase() = delete;

        explicit ObjectBase(const std::string &name, DrawMode mode = DrawMode::DYNAMIC)
        :Object(name), m_drawMode(mode){
            m_VAO = GLVertexArray();
            m_drawMode = mode;
        }

        void SetPipelineData() override = 0;

        void SetupVerticesBuffer() override = 0;

        void SetMaterial() override = 0;

        void BindVAO() override{
            m_VAO.Bind();
        }

        void Draw() override {
            glDrawElements(GL_TRIANGLES, m_vertices.size(), GL_UNSIGNED_INT, nullptr);
        }

    public:
        std::vector<VertexType> m_vertices;
        std::vector<unsigned int> m_indices;
        GLVertexArray m_VAO{};
        DrawMode m_drawMode ;
    };





}


#endif //GAMEPHYSICSINONEWEEKEND_OBECTBASE_H
