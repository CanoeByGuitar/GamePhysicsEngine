//
// Created by 王晨辉 on 2023/6/3.
//

#ifndef GAMEPHYSICSINONEWEEKEND_RESOURCESMANAGER_H
#define GAMEPHYSICSINONEWEEKEND_RESOURCESMANAGER_H

#include <string>
#include <filesystem>
#include <Geometry/Geometry.h>


class ResourceManager{
public:
    static auto& GetInstance(){
        static ResourceManager instance;
        return instance;
    }

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator= (const ResourceManager&) = delete;

    void ReleaseAllResources();

    // Load a text file and return as a string
    std::string LoadTextFile(const std::filesystem::path& path) const;

    geo::Model& LoadModelFileNoMaterial(const std::filesystem::path& path) const;

private:
    ResourceManager() = default;
    ~ResourceManager() = default;

};

#endif //GAMEPHYSICSINONEWEEKEND_RESOURCESMANAGER_H
