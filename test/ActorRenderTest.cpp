//
// Created by 王晨辉 on 2023/6/7.
//

//
// Created by 王晨辉 on 2023/6/4.
//

#include "Utils.h"

using namespace renderer;
class MyGui : public GuiSystem {
   public:
    void Draw(GLFWwindow* window) override {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |=
            ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(
            ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y),
            ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(250, 300), ImGuiCond_Once);
        ImGui::Begin("Convex Hull");
        if (ImGui::IsWindowHovered()) {
            Input::GetInstance().disableMouse();
        } else {
            Input::GetInstance().enableMouse();
        }
        ImGui::Text("Application average %.3f ms/frame\n (%.1f FPS)",
                    1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};

int main() {
    std::vector<Actor*> world;
    auto nameObjectMap = GenWorldFromConfig("resource/config/test.json");
    for (auto& it : nameObjectMap) {
        world.push_back(it.second);
    }
    auto gui = new MyGui;
    Engine engine(world, gui);
    engine.Init();
    engine.Execute();
}