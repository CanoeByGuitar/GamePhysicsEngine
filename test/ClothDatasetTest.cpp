
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

std::unordered_map<std::string, Actor*> GenWorldFromConfig(
    const std::filesystem::path& path) {
    std::unordered_map<std::string, Actor*> world;
    auto config = ResourceManager::GetInstance().LoadJsonFile(path);
    for (const auto& item : config) {
        auto attr = item.second;

        Actor* actor = nullptr;
        ///////// obj renderer settings
        auto drawMode = DYNAMIC;
        auto primitiveType = TRIANGLE;
        if (std::get<std::string>(attr["drawMode"]) == "dynamic") {
            drawMode = DYNAMIC;
        } else {
            drawMode = STATIC;
        }
        if (std::get<std::string>(attr["primitiveType"]) == "triangle") {
            primitiveType = PrimitiveType::TRIANGLE;
        } else {
            primitiveType = PrimitiveType::LINE;
        }

        //////// obj type
        auto type = std::get<std::string>(attr["type"]);
        if (type == "cube") {
            auto geoCube =
                std::make_shared<geo::AABB>(std::get<vec3>(attr["pos"]),
                                            std::get<vec3>(attr["halfSize"]));
            actor = new AabbActor(geoCube,
                                  std::get<std::string>(attr["shader_name"]));
            actor->m_renderComponent->drawMode = drawMode;
            actor->m_renderComponent->primitiveType = primitiveType;
            actor->InitRenderObject();
            actor->m_renderComponent->objects[0]->m_material = std::make_shared<Material>();
            actor->m_renderComponent->objects[0]->m_material->m_color = std::get<vec3>(attr["color"]);
        } else if (type == "model") {
            auto modelPath = std::get<std::string>(attr["model_path"]);
            auto materialModel =
                ResourceManager::GetInstance().LoadModelFileWithMaterial(
                    modelPath, true);
            // resources/models/Marry/Marry.obj ==> resources/models/Marry
            std::string::iterator it = modelPath.end();
            while(*it != '/'){
                it--;
            }
            modelPath.erase(it, modelPath.end());


            auto geoModel = materialModel.model;
            actor = new ModelActor(
                geoModel, std::get<std::string>(attr["shader_name"]));
            actor->m_renderComponent->drawMode = drawMode;
            actor->m_renderComponent->primitiveType = primitiveType;
            actor->InitRenderObject();
            auto objects = actor->m_renderComponent->objects;
            for(int i = 0; i < objects.size(); i++){
                objects[i]->m_material = std::make_shared<Material>();
                objects[i]->m_material->m_indexOfModel = i;
                objects[i]->m_material->m_texturePath = std::filesystem::path(modelPath)/ materialModel.map_kd[i];
                objects[i]->m_material->m_color = materialModel.Kd[i];
                objects[i]->m_coords =  materialModel.textureCoords[i];
                objects[i]->m_normals =  materialModel.normalCoords[i];

            }
        } else if (type == "sphere") {
            auto geoSphere =
                std::make_shared<geo::Sphere>(std::get<vec3>(attr["center"]),
                                              std::get<float>(attr["radius"]));
            actor = new SphereActor(geoSphere,
                                    std::get<std::string>(attr["shader_name"]));
            actor->m_renderComponent->drawMode = drawMode;
            actor->m_renderComponent->primitiveType = primitiveType;
            actor->InitRenderObject();
            actor->m_renderComponent->objects[0]->m_material = std::make_shared<Material>();
            actor->m_renderComponent->objects[0]->m_material->m_color = std::get<vec3>(attr["color"]);
        }
        PHY_ASSERT(actor, "Config error, Actor is null!")



        world[item.first] = actor;
    }
    return world;
}

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
