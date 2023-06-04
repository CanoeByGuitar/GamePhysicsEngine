//
// Created by 王晨辉 on 2023/6/3.
//

#include "GLShaderProgram.h"
#include <Base/Log.h>

namespace renderer{
    GLShaderProgram::GLShaderProgram(const std::string& programName, const GLuint programID)
    : m_programID(programID), m_programName(programName){
        getUniforms();
    }

    GLShaderProgram::~GLShaderProgram() {
        DeleteProgram();
    }

    void GLShaderProgram::Bind() const {
        PHY_ASSERT(m_programID != 0, "Shader program id should not be zero!");
        glUseProgram(m_programID);
    }

    void GLShaderProgram::DeleteProgram() const {
        if(m_programID != 0){
            PHY_INFO("Deleting shader program: {}", m_programName);
            glDeleteProgram(m_programID);
        }
    }

    void GLShaderProgram::SetUniformi(const std::string& uniformName, const int value) const {
        glUniform1i(m_uniforms.at(uniformName), value);
    }


    void GLShaderProgram::SetUniformf(const std::string& uniformName, const float value) const  {
        glUniform1f(m_uniforms.at(uniformName), value);
    }

    void GLShaderProgram::SetUniform(const std::string& uniformName, const glm::ivec2& value) const {
        glUniform2iv(m_uniforms.at(uniformName), 1, &value[0]);
    }

    void GLShaderProgram::SetUniform(const std::string& uniformName, const glm::vec2& value) const {
        glUniform2f(m_uniforms.at(uniformName), value.x, value.y);
    }

    void GLShaderProgram::SetUniform(const std::string& uniformName, const glm::vec3& value)const {
        glUniform3f(m_uniforms.at(uniformName), value.x, value.y, value.z);
    }

    void GLShaderProgram::SetUniform(const std::string& uniformName, const glm::vec4& value) const{
        glUniform4f(m_uniforms.at(uniformName), value.x, value.y, value.z, value.w);
    }

    void GLShaderProgram::SetUniform(const std::string& uniformName, const glm::mat3x3& value)const {
        glUniformMatrix3fv(m_uniforms.at(uniformName), 1, GL_FALSE, value_ptr(value));
    }

    void GLShaderProgram::SetUniform(const std::string& uniformName, const glm::mat4x4& value)const {
        glUniformMatrix4fv(m_uniforms.at(uniformName), 1, GL_FALSE, value_ptr(value));
    }


    void GLShaderProgram::getUniforms() {
        int total = -1;
        glGetProgramiv(m_programID, GL_ACTIVE_UNIFORMS, &total);
        for(auto i = 0; i < total; i++){
            auto name_len = -1, num = -1;
            GLenum type = GL_ZERO;
            char name[100];
            glGetActiveUniform(m_programID, i, sizeof(name) - 1,
                               &name_len, &num, &type, name);
            name[name_len] = 0;
            const auto nameStr = std::string(name);

            m_uniforms.try_emplace(nameStr, glGetUniformLocation(m_programID, name));
        }
    }
}