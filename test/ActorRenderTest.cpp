//
// Created by 王晨辉 on 2023/6/7.
//

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
    std::vector<Actor*> world;

    /////// cube
    auto aabb = std::make_shared<geo::AABB>(vec3(0, 3, 0), vec3(1.f));
    auto actor_cube = new ActorBase<geo::AABB>("test", aabb);
    world.push_back(actor_cube);
        ///// physic attribute
    actor_cube->InitPhysicsObject();
        ///// render attribute
    actor_cube->InitRenderObject();
    actor_cube->m_renderComponent->SetDrawMode(DrawMode::DYNAMIC);
    actor_cube->m_renderComponent->SetPrimitiveType(PrimitiveType::LINE);





    ////// model
    auto model = std::make_shared<geo::Model>(
            ResourceManager::GetInstance().LoadModelFileNoMaterial(
                    "/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/Marry/Marry.obj"
            ));
    auto actor_model = new ActorBase<geo::Model>("test", model);
    world.push_back(actor_model);
        ///// physic attribute

        ///// render attribute
    actor_model->InitRenderObject();
    actor_model->m_renderComponent->SetDrawMode(DrawMode::STATIC);
    actor_model->m_renderComponent->SetPrimitiveType(PrimitiveType::LINE);
    actor_model->m_renderComponent->SetColor({0.f, 0.8f, 0.6f});




    Engine engine(world);
    engine.Execute();
}