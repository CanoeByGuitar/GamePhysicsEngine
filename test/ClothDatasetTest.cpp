
//
// Created by 王晨辉 on 2023/6/4.
//

#include <Base/Color.h>
#include <Engine.h>
#include <Loader/ResourcesManager.h>
#include <Renderer/Objects/Cube.h>
#include <Renderer/Objects/Mesh.h>
#include <Renderer/Scene.h>
#include <glm/gtc/type_ptr.hpp>
#include <variant>
#include "Utils.h"

using namespace renderer;

namespace control {}

namespace control {
extern vec4 clear_color;
extern geo::BVHSplitStrategy bvh_strategy;
extern bool start;
extern bool restart;
extern vec3 center;
extern float radius;
extern geo::AABB scene_bound;
}  // namespace control

std::shared_ptr<geo::Sphere> ball_geo;
bool isBallMoving = false;
float position = 0.2;

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

        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin(
            "Cloth");  // Create a window called "Hello, world!" and append into it.

        if (ImGui::Button("start/stop")) {
            control::start = !control::start;
        }

        if (ImGui::Button("restart")) {
            control::restart = true;
        }

        if (ImGui::Button("move ball")) {
            isBallMoving = !isBallMoving;
        }
        if (isBallMoving) {
            //            control::center.x = Input::GetInstance().GetMouseX();
            //            control::center.y = Input::GetInstance().GetMouseY();
            //            ball_geo->position = control::center;

            ImGui::SliderFloat("ball center z", &position, 2, -2);
            control::center = vec3(0.5, 0.5, position);
            ball_geo->position = control::center;
        }
        static float radius;
        ImGui::SliderFloat("ball radius", &radius, 0.01, 0.2);
        control::radius = radius;
        ball_geo->radius = control::radius;

        if (ImGui::IsWindowHovered()) {
            Input::GetInstance().disableMouse();
        } else {
            Input::GetInstance().enableMouse();
        }

        ImGui::ColorEdit3("clear color", (float*)&control::clear_color);

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
    PHY_LEVEL_DEBUG
    //////// world
    std::vector<Actor*> world;
    world.reserve(300);

    auto nameObjectMap = GenWorldFromConfig("resource/config/cloth.json");
    for (auto& it : nameObjectMap) {
        world.push_back(it.second);
    }
    // Physic solver
    nameObjectMap["cloth"]->InitPhysicsObject();
    ball_geo = std::shared_ptr<geo::Sphere>(
        reinterpret_cast<geo::Sphere*>(nameObjectMap["ball"]->m_geometryCopy));

    auto gui = new MyGui;
    Engine engine(world, gui);
    engine.Init();

    auto pos = control::scene_bound.position;
    PHY_INFO("camera pos: {}", pos);
    engine.SetCameraPosition({pos.x, pos.y, 10});

    engine.Execute();
}
