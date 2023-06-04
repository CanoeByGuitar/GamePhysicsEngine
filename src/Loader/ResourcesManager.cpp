//
// Created by 王晨辉 on 2023/6/3.
//

#include <fstream>
#include "ResourcesManager.h"
#include <Base/Log.h>

void ResourceManager::ReleaseAllResources() {

}

std::string ResourceManager::LoadTextFile(const std::filesystem::path &path) const {
    std::ifstream in(path, std::ios::in);
    PHY_INFO("File path: {} ", path.string());
    PHY_ASSERT(in, "Resource Manager: File loading error")

    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}
