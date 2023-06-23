//
// Created by 王晨辉 on 2023/6/23.
//

#include <Geometry/ParticleSystem.h>
#include <iostream>

#include <Engine.h>
#include <Renderer/Scene.h>
#include <Renderer/Objects/Cube.h>
#include <Renderer/Objects/Mesh.h>
#include <Loader/ResourcesManager.h>
#include <Base/Color.h>
#include <glm/gtc/type_ptr.hpp>
#include <variant>
#include <random>

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
        ImGui::Begin("Convex Hull");
        if (ImGui::IsWindowHovered()) {
            Input::GetInstance().disableMouse();
        } else {
            Input::GetInstance().enableMouse();
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


int main(){
    //////// world
    std::vector<Actor *> world;

    int test_case = 0;

    ////// one layer
//    std::vector<vec2> points =  {
//            {0, 0},
//            {1, 1},
//            {2, 0},
//            {0, 3},
//            {1, 2},
//            {3, 1}};
//    geo::Particles2D particles(points);
//    auto chIdx = particles.ComputeConvexHull();
//    std::vector<vec3> ch(chIdx.size());
//    for(int i = 0; i < chIdx.size(); i++){
//        ch[i] = vec3(points[chIdx[i]], 0);
//    }
//    Actor* actor_ch = new ActorBase<geo::Particles3D>("test",
//                                                   std::make_shared<geo::Particles3D>(ch));
//    actor_ch->InitRenderObject();
//    actor_ch->m_renderComponent->SetPrimitiveType(renderer::LINE);
//    actor_ch->m_renderComponent->SetDrawMode(renderer::STATIC);
//    actor_ch->m_renderComponent->SetColor(vec3(0, 1, 0));
//    world.push_back(actor_ch);

    ////// multi layer
    int num = 100000;
    std::vector<vec2> points;
    for(int i = 0; i < num; i++){
        points.push_back({
            -3 + (float)std::rand() / RAND_MAX * 6,
            -3 + (float)std::rand() / RAND_MAX * 6
        });
    }
    geo::Particles2D particles(points);
    TICK(ConvexHull)
    auto chs = particles.ComputeMultiLayerConvexHull();
    TOCK(ConvexHull)

//    TICK(ConvexHullO2)
//    auto chs = particles.ComputeMultiLayerConvexHullOn2();
//    TOCK(ConvexHullO2)
    int level = 0;
    for(const auto& ch : chs){
        Actor* actor_ch = new ActorBase<geo::Particles3D>("test",
                                                          std::make_shared<geo::Particles3D>(ch));
        actor_ch->InitRenderObject();
        actor_ch->m_renderComponent->SetPrimitiveType(renderer::LINE);
        actor_ch->m_renderComponent->SetDrawMode(renderer::STATIC);
        actor_ch->m_renderComponent->SetColor(ColorMap(level, 0, (int)chs.size() - 1));
        world.push_back(actor_ch);
        level++;
    }




    geo::Particles2D p2(points);
    auto p3 = p2.to3D();
    Actor* actor = new ActorBase<geo::Particles3D>("test",std::make_shared<geo::Particles3D>(p3));
    actor->InitRenderObject();
    actor->m_renderComponent->SetPrimitiveType(renderer::TRIANGLE);
    actor->m_renderComponent->SetDrawMode(renderer::STATIC);
    actor->m_renderComponent->SetColor(vec3(1, 0, 0));
    world.push_back(actor);


    auto gui = new MyGui;
    Engine engine(world, gui);
    engine.Init();
    engine.Execute();
}