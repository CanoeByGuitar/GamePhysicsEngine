//
// Created by 王晨辉 on 2023/6/3.
//

#include <fstream>
#include "ResourcesManager.h"
#include <Base/Log.h>
#include "OBJ_Loader.h"

void ResourceManager::ReleaseAllResources() {

}

std::string ResourceManager::LoadTextFile(const std::filesystem::path &path) const {
    std::ifstream in(path, std::ios::in);
    PHY_INFO("File path: {} ", path.string());
    PHY_ASSERT(in, "Resource Manager: File loading error")

    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

geo::Model& ResourceManager::LoadModelFileNoMaterial(const std::filesystem::path &path) const {
    // *.obj file
    auto loader = objl::Loader();
    auto ret = new geo::Model;
    PHY_INFO("Load Model at : {}", std::string(std::filesystem::current_path()/"../../" / path));
    if(loader.LoadFile(std::filesystem::current_path()/"../../" / path)){
        auto model = loader.LoadedMeshes;
        for(const auto& mesh : model){
            const auto& vertices = mesh.Vertices;
            const auto& indices = mesh.Indices;
            auto geoMesh = geo::Mesh();
            for(int i = 0; i < indices.size(); i += 3){
                auto tri = new geo::Triangle(
                        {vertices[indices[i]].Position.X,
                         vertices[indices[i]].Position.Y,
                         vertices[indices[i]].Position.Z},

                        {vertices[indices[i + 1]].Position.X,
                         vertices[indices[i + 1]].Position.Y,
                         vertices[indices[i + 1]].Position.Z},

                        {vertices[indices[i + 2]].Position.X,
                         vertices[indices[i + 2]].Position.Y,
                         vertices[indices[i + 2]].Position.Z}
                                                      );
                geoMesh.triangles.push_back(tri);
            }
            geoMesh.bound = geo::GetBound(geoMesh.triangles);
            ret->m_meshes.push_back(geoMesh);
        }
    }else{
        PHY_ERROR("Cannot load model file!");
    }

    return *ret;
}



using JsonValueType = std::variant<vec3, float, std::string>;
using ObjectsDictType = std::unordered_map<std::string, JsonValueType>;
std::unordered_map<std::string, ObjectsDictType> ResourceManager::LoadJsonFile(const std::filesystem::path &path) const {
    PHY_INFO("Load Json at : {}", std::string(std::filesystem::current_path()/"../../" / path));
    std::ifstream f(std::filesystem::current_path()/"../../" / path);
    json data = json::parse(f);

    std::unordered_map<std::string, ObjectsDictType> config;
    for(const auto& obj : data["objects"]){
        ObjectsDictType dict;
        std::string name;
        for(auto it = obj.begin(); it != obj.end(); it++){
            const auto& key = it.key();
            const auto& value = it.value();
            if(key == "name"){
                name = value;
            }else{
                if(value.is_array()){
                    if(value.size() == 3){
                        vec3 temp;
                        temp.x = value[0];
                        temp.y = value[1];
                        temp.z = value[2];
                        dict[key] = temp;
                    }else if(value.size() == 4){
                        vec4 temp;
                        temp.x = value[0];
                        temp.y = value[1];
                        temp.z = value[2];
                        temp.w = value[3];
                        dict[key] = temp;
                    }
                }else if(value.is_string()){
                    dict[key] = value.get<std::string>();
                }else if(value.is_number_float()){
                    dict[key] = value.get<float>();
                }
            }
        }
        config[name] = dict;
    }

    return config;
}
