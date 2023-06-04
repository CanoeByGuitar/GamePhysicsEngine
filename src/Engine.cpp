//
// Created by 王晨辉 on 2023/6/1.
//

#include "Engine.h"

void Engine::connectWindowInstanceToInput(GLFWwindow *window) {
    const auto resizeCallback = [](GLFWwindow* w, auto width, auto height) {
        Input::GetInstance().windowResized(width, height);
    };
    glfwSetWindowSizeCallback(window, resizeCallback);

    const auto keyCallback = [](GLFWwindow* w, auto key, auto scancode, auto action, auto mode) {
        Input::GetInstance().keyPressed(key, scancode, action, mode);
    };
    glfwSetKeyCallback(window, keyCallback);

    const auto cursorPosCallback = [](GLFWwindow* w, auto xPos, auto yPos) {
        Input::GetInstance().mouseMoved(xPos, yPos);
    };
    glfwSetCursorPosCallback(window, cursorPosCallback);
}

float framesPerSecond(const float frameTimeMs){
    return 1.0f / (frameTimeMs / 1000.0f);
}



Engine::Engine(const std::shared_ptr<Scene> &scene) {
    PHY_INFO("Engine starting up...");

    PHY_INFO("Initializing window...");
    auto* window = m_window.Init();
    connectWindowInstanceToInput(window);

    PHY_INFO("Initializing renderer...");
    m_renderer.Init();
    m_renderer.SetProjectionMatrix(m_camera);

    m_scene = scene;
}

void Engine::Execute() {
    while (!m_window.ShouldClose()){
        m_window.Update();
        const auto& [width, height] = m_window.GetFramebufferDims();

        auto dt = 0.016;
        m_camera.Update(dt);
        m_renderer.Update(m_camera);

        auto renderList = m_scene->GetObjects();
        m_renderer.Render(m_camera, renderList.cbegin(), renderList.cend());

        m_window.SwapBuffers();
    }
}
