//
// Created by 王晨辉 on 2023/7/27.
//

#include "Engine.h"
#include <Actor.h>
#include <Geometry/TetrahedronMesh.h>
#include <Loader/TetLoader.h>

namespace control {
static PrimitiveType primitiveType = PrimitiveType::TRIANGLE;
}

static Actor* actor;

class MyGui : public GuiSystem {
public:
  void Draw(GLFWwindow* window) override {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y),
                            ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2(250, 300), ImGuiCond_Once);
    ImGui::Begin("Tet");
    if (ImGui::IsWindowHovered()) {
      Input::GetInstance().disableMouse();
    } else {
      Input::GetInstance().enableMouse();
    }

    //// switch between line primitive and fill primitive
//    if (ImGui::Button("switch")) {
//      if (control::primitiveType == PrimitiveType::TRIANGLE) {
//        control::primitiveType = PrimitiveType::LINE;
//      } else {
//        control::primitiveType = PrimitiveType::TRIANGLE;
//      }
//      actor->InitRenderObject(renderer::DYNAMIC, control::primitiveType);
//    }


    ImGui::Text(
      "Application average %.3f ms/frame\n (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
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
  PHY_LEVEL_DEBUG;
  auto tet =
    TetLoader::Load("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/tet.txt");
  auto geoMesh = tet->ToGeoMesh();
  auto model   = std::make_shared<geo::Model>();
  model->m_meshes.push_back(*geoMesh);
  std::vector<Actor*> world;

  actor = new ModelActor(model, "normal_map");
  world.push_back(actor);
  actor->InitRenderObject(DrawMode::DYNAMIC, PrimitiveType::TRIANGLE);
  actor->SetRenderColor({1, 0, 0});

  /// normal
  std::vector<std::vector<vec3>> normals(1);
  for (const auto& face : tet->m_faces) {
    // each triangle has 3 vertices, each vertex has a normal
    for (int i = 0; i < 3; i++) {
      normals[0].push_back(face.m_normal);
    }
  }
  actor->m_renderComponent->SetNormals(normals);

  auto   gui = new MyGui;
  Engine engine(world, gui);
  engine.Init();
  engine.Execute();
}