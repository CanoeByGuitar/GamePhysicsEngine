//
// Created by 王晨辉 on 2023/6/4.
//

#include <Engine.h>
#include <Renderer/Scene.h>
#include <Renderer/Objects/Cube.h>

using namespace renderer;

int main(){
    auto scene = std::make_shared<Scene>();
    auto cube = std::make_shared<Cube>("test", geo::AABB({0, 0, 0}, {1, 1, 1}));
    scene->AddObject(cube);

    Engine engine(scene);
    engine.Execute();
}