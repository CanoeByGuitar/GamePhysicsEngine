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
#include "Utils.h"

using namespace renderer;


std::shared_ptr<geo::AABB> ground_geo;
AabbActor* actor_ground;

std::unordered_map<std::shared_ptr<Object> , int> BVHLevelMap;
std::unordered_map<geo::BVHNode*, std::shared_ptr<Object>> NodeMap;

namespace control{
    static geo::BVHNode* currNode;
    static vec3 ground_color = {0.2, 0.4, 0.6};
    static vec3 ground_pos = vec3(0, -1.2, 0);
    static vec3 ground_halfSize = vec3(100, 1.2, 100);
    static int show_level = 10;
}

namespace control{
    extern vec4 clear_color;
    extern geo::BVHSplitStrategy bvh_strategy;
}


geo::BVHNode* originNode;



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

        ImGui::Begin("BVH");                          // Create a window called "Hello, world!" and append into it.

        if (ImGui::IsWindowHovered()) {
            Input::GetInstance().disableMouse();
        } else {
            Input::GetInstance().enableMouse();
        }

        ImGui::Text("BVH.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat3("ground_pos", glm::value_ptr(control::ground_pos), -3, 3);
        ground_geo->position = control::ground_pos;

        ImGui::DragFloat3("ground_halfSize", glm::value_ptr(control::ground_halfSize), 0.2);
        ground_geo->halfSize = control::ground_halfSize;





        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float *) &control::clear_color);
//        PHY_DEBUG("clear color in gui: {} in addr: ", control::clear_color);
//        std::cout << &control::clear_color << std::endl;
        ImGui::ColorEdit3("ground color", (float *) &control::ground_color);
        actor_ground->m_renderComponent->objects[0]->m_material->m_color = control::ground_color;

        static bool isMultiBVHShow = false;

        ImGui::Checkbox("isMultiBVHShow", &isMultiBVHShow);
        if(isMultiBVHShow){
            ImGui::SliderInt("bvhLevel", &control::show_level, 0, 20);
            for(const auto& item : BVHLevelMap){
                if(item.second != control::show_level){
                    item.first->m_isVisible = false;
                }else{
                    item.first->m_isVisible = true;
                }
            }
        }else{
            if (ImGui::Button("NextNode")) {
                for(const auto& item : BVHLevelMap){
                    item.first->m_isVisible = false;
                }
                if(!control::currNode) control::currNode = originNode;
                else{
                    NodeMap[control::currNode]->m_isVisible = true;
                    control::currNode = control::currNode->left;
                }
                counter++;
            }
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
        }


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




int main() {
    PHY_LEVEL_DEBUG
    //////// world
    std::vector<Actor *> world;
    world.reserve(300);

    auto nameObjectMap = GenWorldFromConfig("resource/config/BVH.json");
    for(auto & it : nameObjectMap){
        world.push_back(it.second);
    }

    ///////// for gui
    ground_geo = std::shared_ptr<geo::AABB>(reinterpret_cast<geo::AABB*>(nameObjectMap["ground"]->m_geometryCopy));
    actor_ground = dynamic_cast<AabbActor *>(nameObjectMap["ground"]);

    /////// BVH
    auto geoModel = reinterpret_cast<geo::Model*>(nameObjectMap["marry"]->m_geometryCopy);
    int index = 0;
    for (auto &mesh: geoModel->m_meshes) {
        geo::AccelerateMesh(mesh);
        std::unordered_map<int, std::vector<geo::BVHNode*>> boundMap;

        geo::TraverseBVH(mesh.accelerator, boundMap);
        int maxDepth = boundMap.size() - 1;

        if(index == 1){
            originNode = boundMap[0][0];
        }
        for (auto &item: boundMap) {
            for (auto &node: item.second) {
                auto aabb = std::make_shared<geo::AABB>(node->bound);
                auto actor_cube = new AabbActor(aabb, "test");
                world.push_back(actor_cube);
                actor_cube->InitRenderObject(DrawMode::STATIC, PrimitiveType::LINE);
                actor_cube->SetRenderColor(ColorMap(item.first, 0, maxDepth));

                BVHLevelMap[actor_cube->m_renderComponent->objects[0]] = item.first;
                NodeMap[node] = actor_cube->m_renderComponent->objects[0];

            }
        }
        index++;
    }

    auto gui = new MyGui;
    Engine engine(world, gui);
    engine.Init();
    engine.Execute();
}

