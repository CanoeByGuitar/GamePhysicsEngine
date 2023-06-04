//
// Created by 王晨辉 on 2023/6/3.
//

#ifndef GAMEPHYSICSINONEWEEKEND_GLSHADERPROGRAM_H
#define GAMEPHYSICSINONEWEEKEND_GLSHADERPROGRAM_H

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>

#include <unordered_map>
#include <string>
namespace renderer{
    class GLShaderProgram{
    public:
        GLShaderProgram(const std::string& programName, const GLuint programID);
        ~GLShaderProgram();

        GLShaderProgram(GLShaderProgram&& other){
            m_uniforms = other.m_uniforms;
            m_programID = other.m_programID;
            m_programName = other.m_programName;

            other.m_uniforms.clear();
            other.m_programID = 0;
            other.m_programName.clear();
        }

        GLShaderProgram& operator=(GLShaderProgram other){
            std::swap(m_uniforms, other.m_uniforms);
            std::swap(m_programID, other.m_programID);
            std::swap(m_programName, other.m_programName);
            return *this;
        }

        // Disable copying to avoid accidentally calling the destructor
        // on the original instance which causes the program to be
        // deleted on the GPU, but all of the original instance's data
        // is dumbly copied over to the new instance (including non-zero
        // programID) but in fact the program ID is now a dangling reference
        GLShaderProgram(const GLShaderProgram&) = delete;
        GLShaderProgram& operator=(const GLShaderProgram&) = delete;

        void Bind() const;
        void DeleteProgram() const;

        void SetUniformi(const std::string& uniformName, const int value) const;
        void SetUniformf(const std::string& uniformName, const float value) const;
        void SetUniform(const std::string& uniformName, const glm::ivec2& value) const;
        void SetUniform(const std::string& uniformName, const glm::vec2& value) const;
        void SetUniform(const std::string& uniformName, const glm::vec3& value) const;
        void SetUniform(const std::string& uniformName, const glm::vec4& value) const;
        void SetUniform(const std::string& uniformName, const glm::mat3x3& value) const;
        void SetUniform(const std::string& uniformName, const glm::mat4x4& value) const;

        auto GetProgramName() const {return m_programName;}
        auto GetProgramID() const {return m_programID;}

    private:
        void getUniforms();

        std::unordered_map<std::string, int> m_uniforms;

        GLuint m_programID = 0;
        std::string m_programName;
    };
}

#endif //GAMEPHYSICSINONEWEEKEND_GLSHADERPROGRAM_H
