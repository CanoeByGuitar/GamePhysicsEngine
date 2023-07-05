//
// Created by 王晨辉 on 2023/7/4.
//

#ifndef GAMEPHYSICSINONEWEEKEND_UTILS_H
#define GAMEPHYSICSINONEWEEKEND_UTILS_H

#include <Engine.h>
#include <Loader/ResourcesManager.h>
#include <Renderer/Objects/Cube.h>
#include <Renderer/Objects/Mesh.h>
#include <Renderer/Scene.h>

inline std::unordered_map<std::string, Actor*> GenWorldFromConfig(
    const std::filesystem::path& path) {
    std::unordered_map<std::string, Actor*> world;
    auto config = ResourceManager::GetInstance().LoadJsonFile(path);
    for (const auto& item : config) {
        auto attr = item.second;

        Actor* actor = nullptr;
        ///////// obj renderer settings
        auto drawMode = renderer::DYNAMIC;
        auto primitiveType = renderer::TRIANGLE;
        if (std::get<std::string>(attr["drawMode"]) == "dynamic") {
            drawMode = renderer::DYNAMIC;
        } else {
            drawMode = renderer::STATIC;
        }
        if (std::get<std::string>(attr["primitiveType"]) == "triangle") {
            primitiveType = renderer::PrimitiveType::TRIANGLE;
        } else {
            primitiveType = renderer::PrimitiveType::LINE;
        }

        //////// obj type
        auto type = std::get<std::string>(attr["type"]);
        if (type == "cube") {
            auto geoCube =
                std::make_shared<geo::AABB>(std::get<vec3>(attr["pos"]),
                                            std::get<vec3>(attr["halfSize"]));
            actor = new AabbActor(geoCube,
                                  std::get<std::string>(attr["shader_name"]));
            actor->InitRenderObject(drawMode, primitiveType);
            actor->SetRenderColor(std::get<vec3>(attr["color"]));
        } else if (type == "model") {
            auto modelPath = std::get<std::string>(attr["model_path"]);
            auto materialModel =
                ResourceManager::GetInstance().LoadModelFileWithMaterial(
                    modelPath, true);
            // resources/models/Marry/Marry.obj ==> resources/models/Marry
            std::string::iterator it = modelPath.end();
            while(*it != '/'){
                it--;
            }
            modelPath.erase(it, modelPath.end());


            auto geoModel = materialModel.model;
            actor = new ModelActor(
                geoModel, std::get<std::string>(attr["shader_name"]));
            actor->InitRenderObject(drawMode, primitiveType);
            auto objects = actor->m_renderComponent->objects;
            for(int i = 0; i < objects.size(); i++){
                objects[i]->m_material->m_indexOfModel = i;
                objects[i]->m_material->m_texturePath = std::filesystem::path(modelPath)/ materialModel.map_kd[i];
                objects[i]->m_material->m_color = materialModel.Kd[i];
                objects[i]->m_coords =  materialModel.textureCoords[i];
                objects[i]->m_normals =  materialModel.normalCoords[i];
            }
            actor->SetRenderColor(std::get<vec3>(attr["color"]));
        } else if (type == "sphere") {
            auto geoSphere =
                std::make_shared<geo::Sphere>(std::get<vec3>(attr["center"]),
                                              std::get<float>(attr["radius"]));
            actor = new SphereActor(geoSphere,
                                    std::get<std::string>(attr["shader_name"]));
            actor->InitRenderObject(drawMode, primitiveType);
            actor->SetRenderColor(std::get<vec3>(attr["color"]));
        }
        PHY_ASSERT(actor, "Config error, Actor is null!")



        world[item.first] = actor;
    }
    return world;
}

#endif  //GAMEPHYSICSINONEWEEKEND_UTILS_H
