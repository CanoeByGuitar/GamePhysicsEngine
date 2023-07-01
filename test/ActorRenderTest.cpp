//
// Created by 王晨辉 on 2023/6/7.
//

//
// Created by 王晨辉 on 2023/6/4.
//

#include <Engine.h>
#include <Renderer/Scene.h>
#include <Renderer/Objects/Cube.h>
#include <Renderer/Objects/Mesh.h>
#include <Loader/ResourcesManager.h>

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

std::unordered_map<std::string, Actor*> GenWorldFromConfig(
    const std::filesystem::path& path) {
    std::unordered_map<std::string, Actor*> world;
    auto config = ResourceManager::GetInstance().LoadJsonFile(path);
    for (const auto& item : config) {
        auto attr = item.second;

        Actor* actor = nullptr;
        //////// obj type
        auto type = std::get<std::string>(attr["type"]);
        if (type == "cube") {
            auto geoCube =
                std::make_shared<geo::AABB>(std::get<vec3>(attr["m_pos"]),
                                            std::get<vec3>(attr["halfSize"]));
            actor = new AabbActor(geoCube,
                                  std::get<std::string>(attr["shader_name"]));
        } else if (type == "model") {
            auto geoModel = std::make_shared<geo::Model>(
                ResourceManager::GetInstance().LoadModelFileNoMaterial(
                    std::get<std::string>(attr["model_path"]), false));
            actor = new ModelActor(geoModel,
                                   std::get<std::string>(attr["shader_name"]));
        } else if (type == "sphere") {
            auto geoSphere =
                std::make_shared<geo::Sphere>(std::get<vec3>(attr["center"]),
                                              std::get<float>(attr["radius"]));
            actor = new SphereActor(geoSphere,
                                    std::get<std::string>(attr["shader_name"]));
        }
        PHY_ASSERT(actor, "Config error, Actor is null!")

        ///////// obj renderer settings
        //        actor->m_renderComponent->SetColor(std::get<vec3>(attr["color"]));
        if (std::get<std::string>(attr["drawMode"]) == "dynamic") {
            actor->m_renderComponent->drawMode = DYNAMIC;
        } else {
            actor->m_renderComponent->drawMode = STATIC;
        }
        if (std::get<std::string>(attr["primitiveType"]) == "triangle") {
            actor->m_renderComponent->primitiveType = PrimitiveType::TRIANGLE;
        } else {
            actor->m_renderComponent->primitiveType = PrimitiveType::LINE;
        }
        actor->InitRenderObject();
        world[item.first] = actor;
    }
    return world;
}


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