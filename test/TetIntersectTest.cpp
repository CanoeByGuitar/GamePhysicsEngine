//
// Created by 王晨辉 on 2023/8/3.
//

//
// Created by 王晨辉 on 2023/7/27.
//

#include "Engine.h"
#include <Actor.h>
#include <Geometry/TetBoolean.h>
#include <Geometry/TetrahedronMesh.h>
#include <Loader/TetLoader.h>

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

  int test_case = 0;

  // parallel face case
  if (test_case == 0) {
    std::vector<Actor*> world;

    auto tet1 =
      TetLoader::Load("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/tet.txt");
    auto tet2 =
      TetLoader::Load("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/tet2.txt");

    SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).GetIntersectionLine();
    auto lineVertices =
      SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).GetIntersectionLineVertices();

    auto line =  SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).m_intersectionLine;
    Actor* line_actor = new ParticlesActor(
      std::make_shared<geo::Particles3D>(line),
      "test");
    line_actor->InitRenderObject(renderer::STATIC, renderer::EDGE);
    line_actor->SetRenderColor({1, 1, 0});
    world.push_back(line_actor);


    Actor* particle_actor = new ParticlesActor(
      std::make_shared<geo::Particles3D>(lineVertices),
      "test");
    particle_actor->InitRenderObject(renderer::STATIC, renderer::TRIANGLE);
    particle_actor->SetRenderColor({1, 1, 0});
    world.push_back(particle_actor);


    auto geoMesh1 = tet1->ToGeoMesh();
    auto model1   = std::make_shared<geo::Model>();
    model1->m_meshes.push_back(*geoMesh1);

    auto                actor1 = new ModelActor(model1, "test");
    actor1->InitRenderObject(DrawMode::DYNAMIC, PrimitiveType::LINE);
    actor1->SetRenderColor({0.7, 0.2, 0});
    /// normal
    std::vector<std::vector<vec3>> normals(1);
    for (const auto& face : tet1->m_faces) {
      // each triangle has 3 vertices, each vertex has a normal
      for (int i = 0; i < 3; i++) {
        normals[0].push_back(face.m_normal);
      }
    }
    actor1->m_renderComponent->SetNormals(normals);





    auto geoMesh2 = tet2->ToGeoMesh();
    auto model2   = std::make_shared<geo::Model>();
    model2->m_meshes.push_back(*geoMesh2);
    auto actor2 = new ModelActor(model2, "test");
    actor2->InitRenderObject(DrawMode::DYNAMIC, PrimitiveType::LINE);
    actor2->SetRenderColor({0, 0.2, 0.6});
    /// normal
    normals[0].clear();
    for (const auto& face : tet2->m_faces) {
      // each triangle has 3 vertices, each vertex has a normal
      for (int i = 0; i < 3; i++) {
        normals[0].push_back(face.m_normal);
      }
    }
    actor2->m_renderComponent->SetNormals(normals);

    world.push_back(actor1);
    world.push_back(actor2);




    auto   gui = new MyGui;
    Engine engine(world, gui);
    engine.Init();
    engine.Execute();
  }

  if (test_case == 1) {
    auto tet1 =
      TetLoader::Load("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/tet2.txt");
    auto tet2 =
      TetLoader::Load("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/tet.txt");

    auto [_x, _newMesh] = SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).AInB();
    auto geoMesh1 =
      SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).ToGeoMeshAinB(_newMesh);

    auto model1 = std::make_shared<geo::Model>();
    model1->m_meshes.push_back(*geoMesh1);
    std::vector<Actor*> world;
    auto                actor1 = new ModelActor(model1, "test");
    actor1->InitRenderObject(DrawMode::DYNAMIC, PrimitiveType::LINE);
    actor1->SetRenderColor({0.7, 0.2, 0});
    /// normal
    std::vector<std::vector<vec3>> normals(1);
    for (const auto& face : tet1->m_faces) {
      // each triangle has 3 vertices, each vertex has a normal
      for (int i = 0; i < 3; i++) {
        normals[0].push_back(face.m_normal);
      }
    }
    actor1->m_renderComponent->SetNormals(normals);

    world.push_back(actor1);

    auto   gui = new MyGui;
    Engine engine(world, gui);
    engine.Init();
    engine.Execute();
  }

  // show line
  if (test_case == 2) {
    std::vector<Actor*> world;

    auto tet1 =
      TetLoader::Load("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/tet.txt");
    auto tet2 =
      TetLoader::Load("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/tet3.txt");

    SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).GetIntersectionLine();
    auto lineVertices =
      SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).GetIntersectionLineVertices();

    auto line =  SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).m_intersectionLine;
    Actor* line_actor = new ParticlesActor(
      std::make_shared<geo::Particles3D>(line),
      "test");
    line_actor->InitRenderObject(renderer::STATIC, renderer::EDGE);
    line_actor->SetRenderColor({1, 1, 0});
    world.push_back(line_actor);


    Actor* particle_actor = new ParticlesActor(
      std::make_shared<geo::Particles3D>(lineVertices),
      "test");
    particle_actor->InitRenderObject(renderer::STATIC, renderer::TRIANGLE);
    particle_actor->SetRenderColor({1, 1, 0});
    world.push_back(particle_actor);


    auto geoMesh1 = tet1->ToGeoMesh();
    auto model1   = std::make_shared<geo::Model>();
    model1->m_meshes.push_back(*geoMesh1);

    auto                actor1 = new ModelActor(model1, "test");
    actor1->InitRenderObject(DrawMode::DYNAMIC, PrimitiveType::LINE);
    actor1->SetRenderColor({0.7, 0.2, 0});
    /// normal
    std::vector<std::vector<vec3>> normals(1);
    for (const auto& face : tet1->m_faces) {
      // each triangle has 3 vertices, each vertex has a normal
      for (int i = 0; i < 3; i++) {
        normals[0].push_back(face.m_normal);
      }
    }
    actor1->m_renderComponent->SetNormals(normals);





    auto geoMesh2 = tet2->ToGeoMesh();
    auto model2   = std::make_shared<geo::Model>();
    model2->m_meshes.push_back(*geoMesh2);
    auto actor2 = new ModelActor(model2, "test");
    actor2->InitRenderObject(DrawMode::DYNAMIC, PrimitiveType::LINE);
    actor2->SetRenderColor({0, 0.2, 0.6});
    /// normal
    normals[0].clear();
    for (const auto& face : tet2->m_faces) {
      // each triangle has 3 vertices, each vertex has a normal
      for (int i = 0; i < 3; i++) {
        normals[0].push_back(face.m_normal);
      }
    }
    actor2->m_renderComponent->SetNormals(normals);

    world.push_back(actor1);
    world.push_back(actor2);




    auto   gui = new MyGui;
    Engine engine(world, gui);
    engine.Init();
    engine.Execute();
  }

  // bunny case
  if (test_case == 3) {
    std::vector<Actor*> world;

    auto tet1 =
      TetLoader::Load("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/bunny_low.mesh",
                      false);
    auto tet2 =
      TetLoader::Load("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/bunny_low.mesh",
                      false,
                      {0.05, 0.05, 0.05});

    {
      SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).GetIntersectionLine();
      auto lineVertices =
        SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).GetIntersectionLineVertices();

      auto line =  SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).m_intersectionLine;
      Actor* line_actor = new ParticlesActor(
        std::make_shared<geo::Particles3D>(line),
        "test");
      line_actor->InitRenderObject(renderer::STATIC, renderer::EDGE);
      line_actor->SetRenderColor({1, 1, 0});
      world.push_back(line_actor);


      Actor* particle_actor = new ParticlesActor(
        std::make_shared<geo::Particles3D>(lineVertices),
        "test");
      particle_actor->InitRenderObject(renderer::STATIC, renderer::TRIANGLE);
      particle_actor->SetRenderColor({1, 1, 0});
      world.push_back(particle_actor);
    }



    {
      auto geoMesh1 = tet1->ToGeoMesh();
      auto model1   = std::make_shared<geo::Model>();
      model1->m_meshes.push_back(*geoMesh1);

      auto                actor1 = new ModelActor(model1, "test");
      actor1->InitRenderObject(DrawMode::DYNAMIC, PrimitiveType::LINE);
      actor1->SetRenderColor({0.7, 0.2, 0});
      /// normal
      std::vector<std::vector<vec3>> normals(1);
      for (const auto& face : tet1->m_faces) {
        // each triangle has 3 vertices, each vertex has a normal
        for (int i = 0; i < 3; i++) {
          normals[0].push_back(face.m_normal);
        }
      }
      actor1->m_renderComponent->SetNormals(normals);
      world.push_back(actor1);
    }





    {
      auto geoMesh2 = tet2->ToGeoMesh();
      auto model2   = std::make_shared<geo::Model>();
      model2->m_meshes.push_back(*geoMesh2);
      auto actor2 = new ModelActor(model2, "test");
      actor2->InitRenderObject(DrawMode::DYNAMIC, PrimitiveType::LINE);
      actor2->SetRenderColor({0, 0.2, 0.6});
      /// normal
      std::vector<std::vector<vec3>> normals(1);
      normals[0].clear();
      for (const auto& face : tet2->m_faces) {
        // each triangle has 3 vertices, each vertex has a normal
        for (int i = 0; i < 3; i++) {
          normals[0].push_back(face.m_normal);
        }
      }
      actor2->m_renderComponent->SetNormals(normals);
      world.push_back(actor2);
    }








    auto   gui = new MyGui;
    Engine engine(world, gui);
    engine.Init();
    engine.Execute();
  }

  // sphere case
  // bunny case
  if (test_case == 4) {
    std::vector<Actor*> world;

    auto tet1 =
      TetLoader::Load("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/sphere.mesh",
                      false);
    auto tet2 =
      TetLoader::Load("/Users/wangchenhui/Dev/GamePhysicsInOneWeekend/resource/models/sphere.mesh",
                      false,
                      vec3{0.9});

    // boolean operator
    {
      SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).GetIntersectionLine();
      auto lineVertices =
        SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).GetIntersectionLineVertices();

      auto line =  SimpleComplex::TetBoolean::GetInstance(tet1.get(), tet2.get()).m_intersectionLine;
      Actor* line_actor = new ParticlesActor(
        std::make_shared<geo::Particles3D>(line),
        "test");
      line_actor->InitRenderObject(renderer::STATIC, renderer::EDGE);
      line_actor->SetRenderColor({1, 1, 0});
      world.push_back(line_actor);


      Actor* particle_actor = new ParticlesActor(
        std::make_shared<geo::Particles3D>(lineVertices),
        "test");
      particle_actor->InitRenderObject(renderer::STATIC, renderer::TRIANGLE);
      particle_actor->SetRenderColor({1, 1, 0});
      world.push_back(particle_actor);
    }



    {
      auto geoMesh1 = tet1->ToGeoMesh();
      auto model1   = std::make_shared<geo::Model>();
      model1->m_meshes.push_back(*geoMesh1);

      auto                actor1 = new ModelActor(model1, "test");
      actor1->InitRenderObject(DrawMode::DYNAMIC, PrimitiveType::LINE);
      actor1->SetRenderColor({0.7, 0.2, 0});
      /// normal
      std::vector<std::vector<vec3>> normals(1);
      for (const auto& face : tet1->m_faces) {
        // each triangle has 3 vertices, each vertex has a normal
        for (int i = 0; i < 3; i++) {
          normals[0].push_back(face.m_normal);
        }
      }
      actor1->m_renderComponent->SetNormals(normals);
      world.push_back(actor1);
    }





    {
      auto geoMesh2 = tet2->ToGeoMesh();
      auto model2   = std::make_shared<geo::Model>();
      model2->m_meshes.push_back(*geoMesh2);
      auto actor2 = new ModelActor(model2, "test");
      actor2->InitRenderObject(DrawMode::DYNAMIC, PrimitiveType::LINE);
      actor2->SetRenderColor({0, 0.2, 0.6});
      /// normal
      std::vector<std::vector<vec3>> normals(1);
      normals[0].clear();
      for (const auto& face : tet2->m_faces) {
        // each triangle has 3 vertices, each vertex has a normal
        for (int i = 0; i < 3; i++) {
          normals[0].push_back(face.m_normal);
        }
      }
      actor2->m_renderComponent->SetNormals(normals);
      world.push_back(actor2);
    }








    auto   gui = new MyGui;
    Engine engine(world, gui);
    engine.Init();
    engine.Execute();
  }

}
