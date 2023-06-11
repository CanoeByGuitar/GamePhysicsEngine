//
// Created by 王晨辉 on 2023/6/2.
//

#ifndef GAMEPHYSICSINONEWEEKEND_GUISYSTEM_H
#define GAMEPHYSICSINONEWEEKEND_GUISYSTEM_H

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"




class GuiSystem{
public:
    GuiSystem() = default;
    virtual ~GuiSystem(){
        shutdown();
    }

    void Init(GLFWwindow* window);

    virtual void Draw(GLFWwindow* window);

    void shutdown(){
//        ImGui_ImplOpenGL3_Shutdown();
//        ImGui_ImplGlfw_Shutdown();
//        ImGui::DestroyContext();
    }

};


#endif //GAMEPHYSICSINONEWEEKEND_GUISYSTEM_H
