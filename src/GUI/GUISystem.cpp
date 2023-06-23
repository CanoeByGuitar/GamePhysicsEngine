//
// Created by 王晨辉 on 2023/6/2.
//

#include "GUISystem.h"
#include <Base/Log.h>
#include <Input.h>

namespace control{
    vec4 clear_color = vec4(0.45f, 0.55f, 0.60f, 1.00f);
}

void GuiSystem::Init(GLFWwindow *window) {
    const char* glsl_version = "#version 150";
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


}

void GuiSystem::Draw(GLFWwindow* window) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    const ImGuiViewport *main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(250, 300), ImGuiCond_Once);
    ImGui::Begin("GUI");
    ImGui::Text("Application average %.3f ms/frame\n (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
