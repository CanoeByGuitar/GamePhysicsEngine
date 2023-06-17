//
// Created by 王晨辉 on 2023/6/15.
//
//
// Created by 王晨辉 on 2023/6/8.
//
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
#include <Base/Color.h>
#include <glm/gtc/type_ptr.hpp>
#include <variant>

using namespace renderer;


namespace control{
}

namespace control{
    extern vec4 clear_color;
    extern geo::BVHSplitStrategy bvh_strategy;
    extern bool start;
}




class MyGui : public GuiSystem {
public:
    void Draw(GLFWwindow *window) override {
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

        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Cloth");                          // Create a window called "Hello, world!" and append into it.

        if (ImGui::Button("start/stop")){
            control::start = !control::start;
        }

        if (ImGui::IsWindowHovered()) {
            Input::GetInstance().disableMouse();
        } else {
            Input::GetInstance().enableMouse();
        }


        ImGui::ColorEdit3("clear color", (float *) &control::clear_color);

        ImGui::Text("Application average %.3f ms/frame\n (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }
};


std::unordered_map<std::string, Actor*> GenWorldFromConfig(const std::filesystem::path& path){
    std::unordered_map<std::string, Actor*> world;
    auto config = ResourceManager::GetInstance().LoadJsonFile(path);
    for(const auto& item : config){
        auto attr = item.second;

        Actor* actor = nullptr;
        //////// obj type
        auto type = std::get<std::string>(attr["type"]);
        if(type == "cube"){
            auto geoCube = std::make_shared<geo::AABB>(
                    std::get<vec3>(attr["pos"]),
                    std::get<vec3>(attr["halfSize"])
            );
            actor = new ActorBase<geo::AABB>(
                    std::get<std::string>(attr["shader_name"]),
                    geoCube);
        }else if(type == "model"){
            auto geoModel = std::make_shared<geo::Model>(
                    ResourceManager::GetInstance().LoadModelFileNoMaterial(
                            std::get<std::string>(attr["model_path"]),
                            true
                    ));
            actor = new ActorBase<geo::Model>(
                    std::get<std::string>(attr["shader_name"]),
                    geoModel);
        }
        PHY_ASSERT(actor, "Config error, Actor is null!")

        ///////// obj renderer settings
        actor->InitRenderObject();
        actor->m_renderComponent->SetColor(std::get<vec3>(attr["color"]));
        if(std::get<std::string>(attr["drawMode"])== "dynamic"){
            actor->m_renderComponent->SetDrawMode(DYNAMIC);
        }else{
            actor->m_renderComponent->SetDrawMode(STATIC);
        }
        if(std::get<std::string>(attr["primitiveType"])== "triangle"){
            actor->m_renderComponent->SetPrimitiveType(PrimitiveType::TRIANGLE);
        }else{
            actor->m_renderComponent->SetPrimitiveType(PrimitiveType::LINE);
        }

        world[item.first] = actor;
    }
    return world;
}



int main() {
    PHY_LEVEL_DEBUG
    //////// world
    std::vector<Actor *> world;
    world.reserve(300);

    auto nameObjectMap = GenWorldFromConfig("resource/config/cloth.json");
    for(auto & it : nameObjectMap){
        world.push_back(it.second);
    }

    // Physic solver
    world[0]->InitPhysicsObject();


    auto gui = new MyGui;
    Engine engine(world, gui);
    engine.Init();
    engine.Execute();
}

