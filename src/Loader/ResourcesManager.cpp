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
                std::unordered_map<vec3, int, Vec3Hash> vertexMap;
                std::vector<vec3> uniqueVertices;
                uniqueVertices.reserve(m_vertices.size());

                for (int i = 0; i < m_indices.size(); i++) {
                    auto vertIdx = m_indices[i];
                    auto it = vertexMap.find(m_vertices[vertIdx]);
                    if (it == vertexMap.end()) {
                        int newIndex = static_cast<int>(uniqueVertices.size());
                        vertexMap[m_vertices[vertIdx]] = newIndex;
                        uniqueVertices.push_back(m_vertices[vertIdx]);
                        m_indices[i] = newIndex;
                    } else {
                        m_indices[i] = vertexMap[m_vertices[vertIdx]];
                    }
                }
                uniqueVertices.shrink_to_fit();
                m_vertices = std::move(uniqueVertices);
                PHY_INFO("after deduplicate: {} vertices, {} indices",
                         m_vertices.size(), m_indices.size());
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

MaterialModel ResourceManager::LoadModelFileWithMaterial(
    const std::filesystem::path& path, bool deduplicated) const {
    // *.obj file
    auto loader = objl::Loader();
    auto retModel = std::make_shared<geo::Model>();
    std::vector<std::vector<vec3>> retNormals;
    std::vector<std::vector<vec2>> retCoords;
    std::vector<std::string> retMapKds; // diffuse maps
    PHY_INFO("Load Model at : {}",
             std::string(std::filesystem::current_path() / "../../" / path));
    if (loader.LoadFile(std::filesystem::current_path() / "../../" / path)) {
        auto model = loader.LoadedMeshes;
        for (const auto& mesh : model) {
            const auto& vertices = mesh.Vertices;
            const auto& indices = mesh.Indices;
            auto map_kd = mesh.MeshMaterial.map_Kd;
            retMapKds.push_back(map_kd);
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
            auto m_normal = std::vector<vec3>(vertices.size());
            auto m_coord = std::vector<vec2>(vertices.size());
            for (int i = 0; i < vertices.size(); i++) {
                m_vertices[i] =
                    vec3(vertices[i].Position.X, vertices[i].Position.Y,
                         vertices[i].Position.Z);
                m_coord[i] = vec2(
                    vertices[i].TextureCoordinate.X,
                    vertices[i].TextureCoordinate.Y
                    );
                m_normal[i] = vec3(
                    vertices[i].Normal.X,
                    vertices[i].Normal.Y,
                    vertices[i].Normal.Z
                    );
            }
            auto m_indices = indices;

            // deduplicate
            if (deduplicated) {
                std::unordered_map<vec3, int, Vec3Hash> vertexMap;
                std::vector<vec3> uniqueVertices;
                std::vector<vec3> uniqueNormal;
                std::vector<vec2> uniqueCoord;
                uniqueVertices.reserve(m_vertices.size());
                uniqueNormal.reserve(m_vertices.size());
                uniqueCoord.reserve(m_vertices.size());
                for (int i = 0; i < m_indices.size(); i++) {
                    auto vertIdx = m_indices[i];
                    auto it = vertexMap.find(m_vertices[vertIdx]);
                    if (it == vertexMap.end()) {
                        int newIndex = static_cast<int>(uniqueVertices.size());
                        vertexMap[m_vertices[vertIdx]] = newIndex;
                        uniqueVertices.push_back(m_vertices[vertIdx]);
                        uniqueNormal.push_back(m_normal[vertIdx]);
                        uniqueCoord.push_back(m_coord[vertIdx]);
                        m_indices[i] = newIndex;
                    } else {
                        m_indices[i] = vertexMap[m_vertices[vertIdx]];
                    }
                }
                uniqueVertices.shrink_to_fit();
                uniqueNormal.shrink_to_fit();
                uniqueCoord.shrink_to_fit();
                m_vertices = std::move(uniqueVertices);
                m_coord = std::move(uniqueCoord);
                m_normal = std::move(uniqueNormal);
                PHY_INFO("after deduplicate: {} vertices, {} indices",
                         m_vertices.size(), m_indices.size());
            }

            geoMesh.vertices = m_vertices;
            geoMesh.indices = m_indices;
            geoMesh.bound = geo::GetBound(geoMesh.triangles);
            retModel->m_meshes.push_back(geoMesh);

            retNormals.push_back(m_normal);
            retCoords.push_back(m_coord);

        }
    } else {
        PHY_ERROR("Cannot load model file!");
    }

    return {retModel, retCoords, retNormals, retMapKds};
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

MaterialModel::MaterialModel(
    const GeoModelPtr& model,
    const std::vector<std::vector<vec2>>& textureCoords,
    const std::vector<std::vector<vec3>>& normalCoords,
    const std::vector<std::string>& mapKd)
    : model(model),
      textureCoords(textureCoords),
      normalCoords(normalCoords),
      map_kd(mapKd) {}
