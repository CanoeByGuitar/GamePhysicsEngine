//
// Created by 王晨辉 on 2023/5/29.
//

#ifndef GAMEPHYSICSINONEWEEKEND_IOUTILS_H
#define GAMEPHYSICSINONEWEEKEND_IOUTILS_H

#include <string>
#include <filesystem>
#include <Base/Log.h>

namespace fs = std::filesystem;

inline std::pair<std::string, std::string>GetShaderFiles(const char* folderPath){
    fs::path folder(folderPath);
    PHY_ASSERT(fs::exists(folder) && fs::is_directory(folder), "Invalid Fold Path!")
    std::string vs, fs;
    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file()) {
            const fs::path& filePath = entry.path();
            std::string extension = filePath.extension().string();
            if (extension == ".vert") {
                vs = filePath.string();
            }else if(extension == ".frag"){
                fs = filePath.string();
            }
        }
    }
    PHY_ASSERT(!vs.empty() && !fs.empty(), "No .vs or .fs found!")
    return {vs, fs};
}

#endif //GAMEPHYSICSINONEWEEKEND_IOUTILS_H
