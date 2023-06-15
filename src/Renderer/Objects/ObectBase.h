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
        explicit Object(const std::string& name,
                        DrawMode mode = DrawMode::DYNAMIC,
                        PrimitiveType type = PrimitiveType::TRIANGLE,
                        vec3 color = vec3(-1))
                        :m_name(name),  m_drawMode(mode), m_primitiveType(type),m_color(color){}
        virtual ~Object() = default;
        virtual void SetPipelineData() = 0;
        virtual void SetupVerticesBuffer() = 0;
        virtual void SetMaterial() = 0;
        virtual void BindVAO() = 0;
        virtual void UnBindVAO() = 0;
        virtual void Draw() = 0;

        void SetColor(const vec3& color){
            m_color = color;
        };

        void SetDrawMode(DrawMode mode){
            m_drawMode = mode;
        }

        void SetPrimitiveType(PrimitiveType type){
            m_primitiveType = type;
        }


        std::string GetObjName() const{
            return m_name;
        }

        DrawMode GetDrawMode() const {
            return m_drawMode;
        }

        PrimitiveType GetPrimitiveType() const {
            return m_primitiveType;
        }

        const vec3& GetColor() const {
            return m_color;
        }


    public:
        bool isVisible = true;


    protected:
        std::string m_name;
        DrawMode m_drawMode;
        PrimitiveType m_primitiveType;
        vec3 m_color;

    };

    template<typename VertexType>
    class ObjectBase : public Object {
    public:
        ObjectBase() = delete;

        explicit ObjectBase(const std::string &name,
                            DrawMode mode = DrawMode::DYNAMIC,
                            PrimitiveType type = PrimitiveType::TRIANGLE,
                            vec3 color = vec3(-1))
        :Object(name, mode, type, color){
            m_VAO = GLVertexArray();
        }

        void SetPipelineData() override = 0;

        void SetupVerticesBuffer() override = 0;

        void SetMaterial() override = 0;

        void BindVAO() override{
            m_VAO.Bind();
        }

        void UnBindVAO() override{
            m_VAO.UnBind();
        }

        void Draw() override {
            if(m_primitiveType == PrimitiveType::LINE){
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }else{
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
        }


    protected:
        std::vector<VertexType> m_vertices;
        std::vector<unsigned int> m_indices;
        GLVertexArray m_VAO{};

    };





}


#endif //GAMEPHYSICSINONEWEEKEND_OBECTBASE_H
