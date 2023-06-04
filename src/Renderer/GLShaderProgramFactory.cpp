//
// Created by 王晨辉 on 2023/6/3.
//

#include <vector>
#include "GLShaderProgramFactory.h"
#include <Base/Log.h>
#include <Loader/ResourcesManager.h>


namespace renderer{
    const std::unordered_map<std::string, int> Type2GL_ENUM{
            { "vertex", GL_VERTEX_SHADER },
            { "fragment", GL_FRAGMENT_SHADER },
            { "geometry", GL_GEOMETRY_SHADER },
            { "compute", GL_ATTACHED_SHADERS}
    };

    void compile(const GLuint id, const GLchar* shaderCode){
        glShaderSource(id, 1, &shaderCode, nullptr);
        glCompileShader(id);
    }

    bool compileStage(const GLuint id, const std::string& shaderCode){
        GLint success = GL_FALSE;
        GLint logLength = -1;

        compile(id, shaderCode.c_str());

        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> infoLog(logLength);
        glGetShaderInfoLog(id, logLength, nullptr, infoLog.data());

        PHY_INFO("{}", std::string(infoLog.cbegin(), infoLog.cend()));

        return success == GL_TRUE;
    }

    bool linkProgram(const GLuint id) {
        GLint success{ GL_FALSE };
        GLint logLength{ -1 };

        glLinkProgram(id);
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> infoLog(logLength);
        glGetProgramInfoLog(id, logLength, nullptr, infoLog.data());
        PHY_INFO("{}", std::string(infoLog.cbegin(), infoLog.cend()));

        return success == GL_TRUE;
    }

    bool validateProgram(const GLuint id) {
        GLint success{ GL_FALSE };
        GLint logLength{ -1 };

        glValidateProgram(id);
        glGetProgramiv(id, GL_VALIDATE_STATUS, &success);
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);

        std::vector<GLchar> infoLog(logLength);
        glGetProgramInfoLog(id, logLength, nullptr, infoLog.data());
        PHY_INFO("{}", std::string(infoLog.cbegin(), infoLog.cend()));

        return success == GL_TRUE;
    }




    std::optional<GLShaderProgram> GLShaderProgramFactory::createShaderProgram(const std::string &programName,const std::vector<ShaderStage> &stages) {
        std::vector<GLuint> shaderIds;
        bool success = true;
        std::unordered_map<int, int> a;
        for(int i = 0; i < stages.size(); ++i){
            int id = glCreateShader(Type2GL_ENUM.at(stages[i].type));
            shaderIds.push_back(id);
            std::string shaderCode = ResourceManager::GetInstance().LoadTextFile(
                    stages[i].filePath);
            if(!compileStage(id, shaderCode)){
                success = false;
                break;
            }
        }

        if(!success){
            for(const auto id : shaderIds){
                glDeleteShader(id);
            }
            return std::nullopt;
        }

        GLuint programID = glCreateProgram();
        for(const auto id : shaderIds){
            glAttachShader(programID, id);
        }

//        if(!linkProgram(programID) || !validateProgram(programID)){
        if(!linkProgram(programID) ){
            for(const auto id : shaderIds){
                glDetachShader(programID, id);
                glDeleteShader(id);
            }
            glDeleteProgram(programID);
            return std::nullopt;
        }

        for(const auto id : shaderIds){
            glDetachShader(programID, id);
            glDeleteShader(id);
        }
        return std::make_optional<GLShaderProgram>({programName, programID});
    }
}