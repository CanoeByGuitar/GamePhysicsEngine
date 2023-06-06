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
    if(loader.LoadFile(path)){
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
            ret->m_meshes.push_back(geoMesh);
        }
    }

    return *ret;
}
