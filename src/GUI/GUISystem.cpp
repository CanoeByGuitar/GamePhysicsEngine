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
    bool show_demo_window = false;
    bool show_another_window = false;


    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // TODO
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls


    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    {
        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(250, 300), ImGuiCond_Once);

        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        if (ImGui::IsWindowHovered())
        {
            // Disable mouse actions by capturing mouse input events
//            ImGui::CaptureMouseFromApp(true);
            Input::GetInstance().disableMouse();
            // Optionally, you can also disable keyboard input events
            // ImGui::CaptureKeyboardFromApp(true);
        }else{
            // Release mouse input events when the mouse is not hovering
//            ImGui::CaptureMouseFromApp(false);
            Input::GetInstance().enableMouse();
            // Optionally, release keyboard input events
            // ImGui::CaptureKeyboardFromApp(false);
        }
//        PHY_DEBUG("[variable] cameraControlEnabled = {}", cameraControlEnabled);

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&control::clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame\n (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);


    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}
