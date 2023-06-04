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


class Engine{
    using Scene = renderer::Scene;
    using RenderSystem = renderer::RenderSystem;
public:
    explicit Engine(const std::shared_ptr<Scene>& scene);
    void Execute();


private:
    static void connectWindowInstanceToInput(GLFWwindow* window);


private:
    WindowSystem m_window;
    RenderSystem m_renderer;

    std::shared_ptr<Scene> m_scene;
    Camera m_camera;
};

#endif //GAMEPHYSICSINONEWEEKEND_ENGINE_H
