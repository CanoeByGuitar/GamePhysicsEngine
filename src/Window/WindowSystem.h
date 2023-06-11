//
// Created by 王晨辉 on 2023/6/2.
//

#ifndef GAMEPHYSICSINONEWEEKEND_WINDOWSYSTEM_H
#define GAMEPHYSICSINONEWEEKEND_WINDOWSYSTEM_H

#include <Base/Log.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowSystem{
public:
    WindowSystem() noexcept = default;

    WindowSystem(WindowSystem&&) = default;
    WindowSystem(const WindowSystem&) = delete;
    WindowSystem& operator=(WindowSystem&&) = default;
    WindowSystem& operator=(const WindowSystem&) = delete;

    ~WindowSystem() = default;

    GLFWwindow* Init();
    void Update();
    void Shutdown() const;

    void SetWindowPos(const std::size_t x, const std::size_t y) const;
    void SwapBuffers() const;

    void EnableCursor() const;
    void DisableCursor() const;

    void SetVsync(const bool vsync) const;

    auto ShouldClose() const noexcept { return m_shouldWindowClose; }
    auto IsCursorVisible() const noexcept { return m_showCursor; }

    // Returns the window's framebuffer dimensions in pixels {width, height}.
    std::pair<int, int> GetFramebufferDims() const;


public:
    GLFWwindow* m_window = nullptr;
    bool m_shouldWindowClose = false;
    bool m_showCursor = false;
};

#endif //GAMEPHYSICSINONEWEEKEND_WINDOWSYSTEM_H
