//
// Created by 王晨辉 on 2023/6/2.
//

#ifndef GAMEPHYSICSINONEWEEKEND_RENDERSYSTEM_H
#define GAMEPHYSICSINONEWEEKEND_RENDERSYSTEM_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "GLShaderProgramFactory.h"
#include "Objects/ObectBase.h"

namespace renderer {
class Scene;
//    class GLShaderProgram;

class RenderSystem {
    using ObjectPtr = std::shared_ptr<Object>;
    using RenderListIterator = std::vector<ObjectPtr>::const_iterator;

   public:
    RenderSystem();

    void Init();

    static void LoadStaticObjects(RenderListIterator renderListBegin,
                                  RenderListIterator renderListEnd);

    void Update(const Camera& camera);

    //Release OpenGl resources
    void Shutdown() const;

    void UpdateView(const Camera& camera);

    void SetProjectionMatrix(const Camera& camera);

    void Render(const Camera& camera, RenderListIterator renderListBegin,
                RenderListIterator renderListEnd);

    void CompileShaders(RenderListIterator renderListBegin,
                        RenderListIterator renderListEnd);


   private:


    void setDefaultState();

    void renderObjectsWithTextures(RenderListIterator renderListBegin,
                                   RenderListIterator renderListEnd) const;

    void renderObjectsNoTextures(RenderListIterator renderListBegin,
                                 RenderListIterator renderListEnd) const;

    void setupTextureSamplers();

   private:
    std::unordered_map<std::string, GLShaderProgram> m_shaderCache;
    size_t m_width{0}, m_height{0};
    glm::mat4 m_projMatrix;
    GLuint m_samplerTexture = 0;
    GLuint m_uboMatrices = 0;
};

}  // namespace renderer

#endif  //GAMEPHYSICSINONEWEEKEND_RENDERSYSTEM_H
