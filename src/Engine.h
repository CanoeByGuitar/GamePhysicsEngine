//
// Created by 王晨辉 on 2023/6/1.
//

#ifndef GAMEPHYSICSINONEWEEKEND_ENGINE_H
#define GAMEPHYSICSINONEWEEKEND_ENGINE_H

#include <glad/glad.h>
#include <memory>
#include <Input.h>
#include <Renderer/Camera.h>

#include <Renderer/RenderSystem.h>
#include <Renderer/Scene.h>
#include <Window/WindowSystem.h>
#include "Actor.h"
#include <Physic/PhysicsSystem.h>
#include <GUI/GUISystem.h>
class Engine{
    using Scene = renderer::Scene;
    using RenderSystem = renderer::RenderSystem;
public:
    explicit Engine(const std::vector<Actor*> &world, GuiSystem* gui = new GuiSystem);
    void Init();
    void Execute();
    void SetCameraPosition(const vec3& pos){
        m_camera.SetPos(pos);
    }


private:
    static void connectWindowInstanceToInput(GLFWwindow* window);


private:
    std::vector<Actor*> m_world;
    std::shared_ptr<Scene> m_scene;

    WindowSystem m_window;
    RenderSystem m_renderer;
    GuiSystem* m_gui;

    PhysicsSystem* m_physics;
    Camera m_camera;
};

#endif //GAMEPHYSICSINONEWEEKEND_ENGINE_H
