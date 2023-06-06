//
// Created by 王晨辉 on 2023/6/1.
//

#include "Engine.h"
#include "timer.h"

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

    const auto mouseClickCallback = [](GLFWwindow* w, auto button, auto action, auto mode){
        Input::GetInstance().mouseClicked(button, action, mode);
    };
    glfwSetMouseButtonCallback(window, mouseClickCallback);
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
    m_scene = scene;
    m_renderer.Init();

    auto renderList = m_scene->GetObjects();
    RenderSystem::LoadStaticObjects(renderList.cbegin(), renderList.cend());

    m_renderer.SetProjectionMatrix(m_camera);

}

void Engine::Execute() {
    bool hasOneSecondPassed = false;
    Timer timer(1.0f, [&](){
        hasOneSecondPassed = true;
    });

    while (!m_window.ShouldClose()){
        timer.Update((float)glfwGetTime());
        if(hasOneSecondPassed){
            // TODO: print some info every one second including FPS
            hasOneSecondPassed = false;
        }
        auto dt = timer.GetDelta();

        m_window.Update();
        const auto& [width, height] = m_window.GetFramebufferDims();

        m_camera.Update(dt);
        m_renderer.Update(m_camera);

        auto renderList = m_scene->GetObjects();
        m_renderer.Render(m_camera, renderList.cbegin(), renderList.cend());

        m_window.SwapBuffers();
    }
}
