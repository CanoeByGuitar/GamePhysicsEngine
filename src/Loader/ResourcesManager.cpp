//
// Created by 王晨辉 on 2023/6/3.
//

#include "ResourcesManager.h"
#include <Base/DataStructure.h>
#include <Base/Log.h>
#include <fstream>
#include "OBJ_Loader.h"

void ResourceManager::ReleaseAllResources() {}

std::string ResourceManager::LoadTextFile(
    const std::filesystem::path& path) const {
    std::ifstream in(path, std::ios::in);
    PHY_INFO("File path: {} ", path.string());
    PHY_ASSERT(in, "Resource Manager: File loading error")

    return std::string(std::istreambuf_iterator<char>(in),
                       std::istreambuf_iterator<char>());
}

geo::Model& ResourceManager::LoadModelFileNoMaterial(
    const std::filesystem::path& path, bool deduplicated) const {
    // *.obj file
    auto loader = objl::Loader();
    auto ret = new geo::Model;
    PHY_INFO("Load Model at : {}",
             std::string(std::filesystem::current_path() / "../../" / path));
    if (loader.LoadFile(std::filesystem::current_path() / "../../" / path)) {
        auto model = loader.LoadedMeshes;
        for (const auto& mesh : model) {
            const auto& vertices = mesh.Vertices;
            const auto& indices = mesh.Indices;
            auto geoMesh = geo::Mesh();
            for (int i = 0; i < indices.size(); i += 3) {
                auto tri =
                    new geo::Triangle({vertices[indices[i]].Position.X,
                                       vertices[indices[i]].Position.Y,
                                       vertices[indices[i]].Position.Z},

                                      {vertices[indices[i + 1]].Position.X,
                                       vertices[indices[i + 1]].Position.Y,
                                       vertices[indices[i + 1]].Position.Z},

                                      {vertices[indices[i + 2]].Position.X,
                                       vertices[indices[i + 2]].Position.Y,
                                       vertices[indices[i + 2]].Position.Z});
                geoMesh.triangles.push_back(tri);
            }
            auto m_vertices = std::vector<vec3>(vertices.size());
            for (int i = 0; i < vertices.size(); i++) {
                m_vertices[i] =
                    vec3(vertices[i].Position.X, vertices[i].Position.Y,
                         vertices[i].Position.Z);
            }
            auto m_indices = indices;

            // deduplicate
            if (deduplicated) {
                std::unordered_map<glm::vec3, int, Vec3Hash> duplicate;
                int duplicateNum = 0;
                std::vector<vec3> newVertices;
                newVertices.reserve(m_vertices.size());
                std::vector<unsigned int> newIndices(m_indices.size());
                for (int i = 0; i < m_vertices.size(); i++) {
                    if (duplicate.find(m_vertices[i]) == duplicate.end()) {
                        newVertices.push_back(m_vertices[i]);
                        newIndices[i] = m_indices[i] - duplicateNum;
                        duplicate[m_vertices[i]] = newIndices[i];
                    } else {
                        newIndices[i] = duplicate[m_vertices[i]];
                        duplicateNum++;
                    }
                }
                PHY_INFO("duplicateNum: {}", duplicateNum);
                newVertices.shrink_to_fit();
                m_vertices.swap(newVertices);
                m_indices.swap(newIndices);
                PHY_INFO(
                    "After deduplicate, there are totally {} vertices and {} "
                    "indices.",
                    m_vertices.size(), m_indices.size());
                //    for(int i = 0; i < m_vertices.size(); i++){
                //        PHY_INFO("({}): {}  ",i, m_vertices[i]);
                //    }

                //                for(int i = 0; i < m_indices.size(); i++){
                //                    PHY_INFO("({}): {}  ",i, m_indices[i]);
                //                }
            }

            geoMesh.vertices = m_vertices;
            geoMesh.indices = m_indices;
            geoMesh.bound = geo::GetBound(geoMesh.triangles);
            ret->m_meshes.push_back(geoMesh);
        }
    } else {
        PHY_ERROR("Cannot load model file!");
    }

    return *ret;
}

using JsonValueType = std::variant<vec3, float, std::string>;
using ObjectsDictType = std::unordered_map<std::string, JsonValueType>;
std::unordered_map<std::string, ObjectsDictType> ResourceManager::LoadJsonFile(
    const std::filesystem::path& path) const {
    PHY_INFO("Load Json at : {}",
             std::string(std::filesystem::current_path() / "../../" / path));
    std::ifstream f(std::filesystem::current_path() / "../../" / path);
    json data = json::parse(f);

    std::unordered_map<std::string, ObjectsDictType> config;
    for (const auto& obj : data["objects"]) {
        ObjectsDictType dict;
        std::string name;
        for (auto it = obj.begin(); it != obj.end(); it++) {
            const auto& key = it.key();
            const auto& value = it.value();
            if (key == "name") {
                name = value;
            } else {
                if (value.is_array()) {
                    if (value.size() == 3) {
                        vec3 temp;
                        temp.x = value[0];
                        temp.y = value[1];
                        temp.z = value[2];
                        dict[key] = temp;
                    } else if (value.size() == 4) {
                        vec4 temp;
                        temp.x = value[0];
                        temp.y = value[1];
                        temp.z = value[2];
                        temp.w = value[3];
                        dict[key] = temp;
                    }
                } else if (value.is_string()) {
                    dict[key] = value.get<std::string>();
                } else if (value.is_number_float()) {
                    dict[key] = value.get<float>();
                }
            }
        }
        config[name] = dict;
    }

    return config;
}
