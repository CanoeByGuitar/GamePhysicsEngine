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
#include <Base/ControlParam.h>

using namespace renderer;



using control::clear_color;

auto ground_geo = std::make_shared<geo::AABB>(control::ground_pos, control::ground_halfSize);
auto actor_ground = new ActorBase<geo::AABB>("test", ground_geo);

std::unordered_map<std::shared_ptr<Object> , int> BVHLevelMap;
std::unordered_map<geo::BVHNode*, std::shared_ptr<Object>> NodeMap;

namespace control{
    geo::BVHNode* currNode;
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


//        ImGui::SliderInt("bvhLevel", &control::Show_Level, 0, 20);
//        for(const auto& item : BVHLevelMap){
//            if(item.second != control::Show_Level){
//                item.first->isVisible = false;
//            }else{
//                item.first->isVisible = true;
//            }
//        }


        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float *) &control::clear_color);
//        PHY_DEBUG("clear color in gui: {} in addr: ", control::clear_color);
//        std::cout << &control::clear_color << std::endl;
        ImGui::ColorEdit3("ground color", (float *) &control::ground_color);
        actor_ground->m_renderComponent->SetColor(control::ground_color);

        if (ImGui::Button("NextNode")) {
            for(const auto& item : BVHLevelMap){
                item.first->isVisible = false;
            }
            if(!control::currNode) control::currNode = originNode;
            else{
                NodeMap[control::currNode]->isVisible = true;
                control::currNode = control::currNode->left;
            }
            counter++;
        }

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

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
    std::vector<Actor *> world;

    /////// Ground
    actor_ground->InitRenderObject();
    actor_ground->m_renderComponent->SetColor(control::ground_color);
    actor_ground->m_renderComponent->SetDrawMode(DrawMode::DYNAMIC);
    actor_ground->m_renderComponent->SetPrimitiveType(PrimitiveType::TRIANGLE);

    world.push_back(actor_ground);

    ////// model
    auto model = std::make_shared<geo::Model>(
            ResourceManager::GetInstance().LoadModelFileNoMaterial(
                    "/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/Marry/Marry.obj"
            ));

    auto actor_model = new ActorBase<geo::Model>("test", model);
    world.push_back(actor_model);
    actor_model->InitRenderObject();
    actor_model->m_renderComponent->SetDrawMode(DrawMode::STATIC);
    actor_model->m_renderComponent->SetPrimitiveType(PrimitiveType::TRIANGLE);
    actor_model->m_renderComponent->SetColor({0.f, 0.8f, 0.6f});


    int index = 0;
    for (auto &mesh: model->m_meshes) {
        geo::AccelerateMesh(mesh);
        std::unordered_map<int, std::vector<geo::BVHNode*>> boundMap;

        geo::TraverseBVH(mesh.accelerator, boundMap);
        int maxDepth = boundMap.size() - 1;

        if(index == 1){
            originNode = boundMap[0][0];
        }

        /////// BVH
        for (auto &item: boundMap) {
            for (auto &node: item.second) {
                auto aabb = std::make_shared<geo::AABB>(node->bound);
                auto actor_cube = new ActorBase<geo::AABB>("test", aabb);
                world.push_back(actor_cube);
                actor_cube->InitRenderObject();
                actor_cube->m_renderComponent->SetColor(ColorMap(item.first, 0, maxDepth));
                actor_cube->m_renderComponent->SetDrawMode(DrawMode::STATIC);
                actor_cube->m_renderComponent->SetPrimitiveType(PrimitiveType::LINE);

                BVHLevelMap[actor_cube->m_renderComponent->object] = item.first;
                NodeMap[node] = actor_cube->m_renderComponent->object;

            }
        }
        index++;
    }



    std::vector<Actor *> test_world(7);
    for (int i = 0; i < 6; i++) {
        test_world.push_back(world[i]);
    }
    test_world.push_back(world.back());

    auto gui = new MyGui;
    Engine engine(world, gui);
    engine.Init();
    engine.Execute();
}

