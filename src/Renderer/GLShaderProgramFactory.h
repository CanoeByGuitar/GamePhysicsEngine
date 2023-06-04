//
// Created by 王晨辉 on 2023/6/3.
//

#ifndef GAMEPHYSICSINONEWEEKEND_GLSHADERPROGRAMFACTORY_H
#define GAMEPHYSICSINONEWEEKEND_GLSHADERPROGRAMFACTORY_H

#include "GLShaderProgram.h"
#include <optional>

namespace renderer {
    struct ShaderStage {
        ShaderStage() = default;

        ShaderStage(const std::string &path,
                    const std::string &type)
                : filePath(path), type(type) {}

        std::string filePath;
        std::string type;
    };

    class GLShaderProgramFactory {
    public:
        static std::optional<GLShaderProgram> createShaderProgram(const std::string& programName,
                                                   const std::vector<ShaderStage>& stages);
    };
}
#endif //GAMEPHYSICSINONEWEEKEND_GLSHADERPROGRAMFACTORY_H
