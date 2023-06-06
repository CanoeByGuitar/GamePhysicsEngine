//
// Created by 王晨辉 on 2023/6/4.
//

#include <Engine.h>
#include <Renderer/Scene.h>
#include <Renderer/Objects/Cube.h>
#include <Renderer/Objects/Mesh.h>
#include <Loader/ResourcesManager.h>

using namespace renderer;

int main() {
    /////// cube
    auto cube = std::make_shared<Cube>("test",
                                       geo::AABB({0, 0, 0}, {1, 1, 1}),
                                       DrawMode::STATIC,
                                       PrimitiveType::LINE,
                                       vec3{-1.f});


    ////// mesh
    auto mesh1 = std::make_shared<BasicMesh>("test",
                                            std::make_shared<geo::Mesh>(
                                                    ResourceManager::GetInstance().LoadModelFileNoMaterial(
                                                            "/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/Marry/Marry.obj"
                                                    ).m_meshes[0]),
                                            DrawMode::STATIC,
                                            PrimitiveType::TRIANGLE);

    auto mesh2 = std::make_shared<BasicMesh>("test",
                                             std::make_shared<geo::Mesh>(
                                                     ResourceManager::GetInstance().LoadModelFileNoMaterial(
                                                             "/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/Marry/Marry.obj"
                                                     ).m_meshes[1]),
                                             DrawMode::STATIC,
                                             PrimitiveType::TRIANGLE);

    ////// model
    auto model = std::make_shared<BasicModel>("test",
                                             std::make_shared<geo::Model>(
                                                     ResourceManager::GetInstance().LoadModelFileNoMaterial(
                                                             "/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/Marry/Marry.obj"
                                                     )),
                                             DrawMode::STATIC,
                                             PrimitiveType::LINE,
                                             vec3{0.f, 0.8f, 0.6f});

    auto scene = std::make_shared<Scene>();
    scene->AddObject(cube);
//    scene->AddObject(mesh1);
//    scene->AddObject(mesh2);
    scene->AddObject(model);

    Engine engine(scene);
    engine.Execute();
}