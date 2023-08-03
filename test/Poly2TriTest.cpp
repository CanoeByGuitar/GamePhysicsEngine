//
// Created by 王晨辉 on 2023/8/3.
//

#include <poly2tri.h>
#include "Engine.h"
#include <Actor.h>
#include <Geometry/TetrahedronMesh.h>
#include <Loader/TetLoader.h>


using namespace std;
using p2t::CDT;

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



int main(){

  vector<p2t::Point*> polyline =
    {new p2t::Point{1, 0},
     new p2t::Point{-1, 0},
     new p2t::Point{0, 4}};


  vector<p2t::Point*> hole=
    {new p2t::Point{0.5, 1},
     new p2t::Point{-0.5, 1},
     new p2t::Point{0, 0}};

  vector<p2t::Point*> steiner =
    {
      new p2t::Point{0.5, 1},
      new p2t::Point{-0.5, 1},
      new p2t::Point{0, 0}};

  CDT* cdt = new CDT(polyline);
//  cdt->AddHole(hole);
  for(auto s : steiner){
    cdt->AddPoint(s);
  }

  cdt->Triangulate();
  vector<p2t::Triangle*> triangles = cdt->GetTriangles();

  GeoMeshPtr geoMesh = std::make_shared<geo::Mesh>();;
  int cnt = 0;
  for(auto tri : triangles){
    p2t::Point* a = tri->GetPoint(0);
    p2t::Point* b = tri->GetPoint(1);
    p2t::Point* c = tri->GetPoint(2);

    geoMesh->vertices.emplace_back(a->x, a->y, 0);
    geoMesh->vertices.emplace_back(b->x, b->y, 0);
    geoMesh->vertices.emplace_back(c->x, c->y, 0);

    for(int i = 0; i < 3; i++){
      geoMesh->indices.push_back(cnt + i);
    }
    cnt += 3;
  }
  auto model   = std::make_shared<geo::Model>();
  model->m_meshes.push_back(*geoMesh);
  std::vector<Actor*> world;
  Actor* actor = new ModelActor(model, "test");
  world.push_back(actor);
  actor->InitRenderObject(DrawMode::DYNAMIC, PrimitiveType::LINE);
  actor->SetRenderColor({1, 0, 0});
  auto   gui = new MyGui;
  Engine engine(world, gui);
  engine.Init();
  engine.Execute();
}