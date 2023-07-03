//
// Created by 王晨辉 on 2023/6/3.
//

#ifndef GAMEPHYSICSINONEWEEKEND_RESOURCESMANAGER_H
#define GAMEPHYSICSINONEWEEKEND_RESOURCESMANAGER_H

#include <string>
#include <filesystem>
#include <Geometry/Geometry.h>
#include "json.hpp"
#include <utility>
#include <variant>

struct MaterialModel{
    // vertices, indices
    GeoModelPtr model;
    // vertices texture coords, normals
    std::vector<std::vector<vec2>> textureCoords;
    std::vector<std::vector<vec3>> normalCoords;

    // texture map
    std::vector<std::string> map_kd; // diffuse map

    MaterialModel(const GeoModelPtr& model,
                  const std::vector<std::vector<vec2>>& textureCoords,
                  const std::vector<std::vector<vec3>>& normalCoords,
                  const std::vector<std::string>& mapKd);
};

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

    geo::Model& LoadModelFileNoMaterial(const std::filesystem::path& path, bool deduplicated = false) const;
    MaterialModel LoadModelFileWithMaterial(const std::filesystem::path& path, bool deduplicated = false) const;

    using json = nlohmann::json;
    using JsonValueType = std::variant<vec3, float, std::string>;
    using ObjectsDictType = std::unordered_map<std::string, JsonValueType>;
    std::unordered_map<std::string, ObjectsDictType> LoadJsonFile(const std::filesystem::path& path) const;

private:
    ResourceManager() = default;
    ~ResourceManager() = default;

};

#endif //GAMEPHYSICSINONEWEEKEND_RESOURCESMANAGER_H
