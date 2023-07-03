//
// Created by 王晨辉 on 2023/6/1.
//

#include "Engine.h"
#include "timer.h"

namespace control {
bool start = false;
}

void Engine::connectWindowInstanceToInput(GLFWwindow* window) {
    const auto resizeCallback = [](GLFWwindow* w, auto width, auto height) {
        Input::GetInstance().windowResized(width, height);
    };
    glfwSetWindowSizeCallback(window, resizeCallback);

    const auto keyCallback = [](GLFWwindow* w, auto key, auto scancode,
                                auto action, auto mode) {
        Input::GetInstance().keyPressed(key, scancode, action, mode);
    };
    glfwSetKeyCallback(window, keyCallback);

    const auto cursorPosCallback = [](GLFWwindow* w, auto xPos, auto yPos) {
        Input::GetInstance().mouseMoved(xPos, yPos);
    };
    glfwSetCursorPosCallback(window, cursorPosCallback);

    const auto mouseClickCallback = [](GLFWwindow* w, auto button, auto action,
                                       auto mode) {
        Input::GetInstance().mouseClicked(button, action, mode);
    };
    glfwSetMouseButtonCallback(window, mouseClickCallback);
}

float framesPerSecond(const float frameTimeMs) {
    return 1.0f / (frameTimeMs / 1000.0f);
}

Engine::Engine(const std::vector<Actor*>& world, GuiSystem* gui)
    : m_gui(gui), m_world(world) {}

void Engine::Init() {
    PHY_INFO("Engine starting up...");

    PHY_INFO("Loading world actors...");

    PHY_INFO("Initializing window...");
    auto* window = m_window.Init();
    connectWindowInstanceToInput(window);

    PHY_INFO("Initializing physics...");
    m_physics = new PhysicsSystem();
    //    m_physics = new ThreeBodySystem();
    for (auto& actor : m_world) {
        if (actor->m_physicsComponent) {
            m_physics->AddObject(actor->m_physicsComponent->object);
        }
    }
    m_physics->Init();

    PHY_INFO("Initializing renderer...");
    m_scene = std::make_shared<Scene>();
    for (auto& actor : m_world) {
        for (const auto& obj : actor->m_renderComponent->objects) {
            m_scene->AddObject(obj);
        }
    }
    auto renderList = m_scene->GetObjects();
    m_renderer.Init();
    m_renderer.CompileShaders(renderList.cbegin(), renderList.cend());
    RenderSystem::LoadStaticObjects(renderList.cbegin(), renderList.cend());

    m_renderer.SetProjectionMatrix(m_camera);

    PHY_INFO("Initializing gui...");
    m_gui->Init(m_window.m_window);
}

void Engine::Execute() {
    bool hasOneSecondPassed = false;
    Timer timer(1.0f, [&]() { hasOneSecondPassed = true; });

    while (!m_window.ShouldClose()) {
        m_window.Update();
        timer.Update((float)glfwGetTime());
        if (hasOneSecondPassed) {
            // TODO: print some info every one second including FPS
            hasOneSecondPassed = false;
        }
        auto dt = timer.GetDelta();
        //        PHY_INFO("dt:{}", dt);
        ///////////// physics
        if (control::start) {
            m_physics->Update(dt);
            m_physics->Finish();
            //            control::start = false;
        }

        ///////////// renderer
        m_camera.Update(dt);
        m_renderer.Update(m_camera);
        auto renderList = m_scene->GetObjects();
        m_renderer.Render(m_camera, renderList.cbegin(), renderList.cend());

        //////////// Gui
        m_gui->Draw(m_window.m_window);

        m_window.SwapBuffers();
    }
}
