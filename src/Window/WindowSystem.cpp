//
// Created by 王晨辉 on 2023/6/2.
//

#include "WindowSystem.h"
#include "../Input.h"

GLFWwindow *WindowSystem::Init() {
    // Gross lambda to connect the Input singleton to GLFW caallbacks
#define genericInputCallback(functionName) \
    [](GLFWwindow* window, const auto... args){ \
        const auto ptr = static_cast<Input*>(glfwGetWindowUserPointer(window)); \
        if(ptr->functionName){ptr -> functionName(args...)}\
    }

    PHY_ASSERT(glfwInit(), "Failed to start GLFW!");

    glfwSetErrorCallback([](const auto errorCode, const auto* message){
        PHY_ERROR("GLFW Error: {}. {}", errorCode, message);
    });
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    const auto width = 1280;
    const auto height = 720;

    m_window = glfwCreateWindow(width, height, "PhysicEngine", nullptr, nullptr);
    PHY_ASSERT(m_window, "Failed to start GLFW!")

    glfwMakeContextCurrent(m_window);
    glfwFocusWindow(m_window);

    return m_window;
}

void WindowSystem::Update() {
    glfwPollEvents();

//    if (Input::GetInstance().IsKeyPressed(GLFW_KEY_TAB)) {
//        m_showCursor = !m_showCursor;
//        if (m_showCursor) {
//            EnableCursor();
//        } else {
//            DisableCursor();
//        }
//
//    }
    // Check if the window needs to be closed
    if (Input::GetInstance().IsKeyPressed(GLFW_KEY_ESCAPE) || glfwWindowShouldClose(m_window)) {
        m_shouldWindowClose = true;
        glfwSetWindowShouldClose(m_window, true);
    }
}

void WindowSystem::Shutdown() const {

}

void WindowSystem::SetWindowPos(const std::size_t x, const std::size_t y) const {

}


void WindowSystem::SwapBuffers() const {
    glfwSwapBuffers(m_window);
}


void WindowSystem::EnableCursor() const {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}


void WindowSystem::DisableCursor() const {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void WindowSystem::SetVsync(const bool vsync) const {

}

std::pair<int, int> WindowSystem::GetFramebufferDims() const {
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);

    return { width, height };
}
