//
// Created by 王晨辉 on 2023/6/2.
//

#include "RenderSystem.h"
#include "GLShaderProgramFactory.h"
#include "Input.h"
#include <Base/ControlParam.h>

using control::clear_color;

namespace renderer{
    void RenderSystem::Init() {
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            spdlog::info("Failed to initialize GLAD");
            std::abort();
        }

        const auto width = 1280;
        const auto height = 720;

        m_width = width;
        m_height = height;

        compileShaders();
        setupTextureSamplers();
        setDefaultState();

        // Create uniform buffer object for projection and view matrices
        // so same data shared to multiple shader programs.
        for(auto const& item : m_shaderCache){
            auto programID = item.second.GetProgramID();
            GLuint uboBlockIndex = glGetUniformBlockIndex(programID, "Matrices");
            glUniformBlockBinding(programID, uboBlockIndex, 0);
        }

        glGenBuffers(1, &m_uboMatrices);
        glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
        glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboMatrices, 0, 2 * sizeof(glm::mat4));
        glBindFramebuffer(GL_FRAMEBUFFER, 0);




        glViewport(0, 0, width, height);
        
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);


    }

    void RenderSystem::LoadStaticObjects(RenderSystem::RenderListIterator renderListBegin,
                                         RenderSystem::RenderListIterator renderListEnd) {
        auto begin = renderListBegin;
        while(begin != renderListEnd){
            auto objectPtr = *begin;
            objectPtr->SetupVerticesBuffer();
            objectPtr->SetPipelineData();
            begin++;
        }
    }

    void RenderSystem::Update(const Camera &camera) {
        // Window size changed.
        if (Input::GetInstance().ShouldResize()) {
            m_width = Input::GetInstance().GetWidth();
            m_height = Input::GetInstance().GetHeight();
            glViewport(0, 0, m_width, m_height);
        }

        // Update view matrix inside UBO
        UpdateView(camera);
    }

    void RenderSystem::Shutdown() const {

    }

    void RenderSystem::UpdateView(const Camera &camera) {
        const auto view = camera.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

    }

    void RenderSystem::Render(const Camera &camera, RenderSystem::RenderListIterator renderListBegin,
                              RenderSystem::RenderListIterator renderListEnd) {
        setDefaultState();
//        PHY_DEBUG("clear color in renderer: {} in addr: ", clear_color);
//        std::cout << &clear_color << std::endl;
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
        glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);

        renderObjectsNoTextures(renderListBegin, renderListEnd);
    }

    void RenderSystem::compileShaders() {
        m_shaderCache.clear();
        std::string name = m_shaders.first;
        auto stages = m_shaders.second;
        auto shaderProgram = GLShaderProgramFactory::createShaderProgram(
                name,
                stages);

        PHY_ASSERT(shaderProgram, "shader compile error!");
        m_shaderCache.try_emplace(name, std::move(shaderProgram.value()));
    }

    void RenderSystem::setDefaultState() {
//        glFrontFace(GL_CCW);
//        glCullFace(GL_BACK);
//        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void
    RenderSystem::renderObjectsWithTextures(GLShaderProgram &shader, RenderSystem::RenderListIterator renderListBegin,
                                            RenderSystem::RenderListIterator renderListEnd) const {

    }



    void RenderSystem::renderObjectsNoTextures(RenderListIterator renderListBegin,
                                               RenderListIterator renderListEnd) const {
        auto begin = renderListBegin;
        while(begin != renderListEnd){
            auto objectPtr = *begin;
            if(objectPtr->isVisible){
                static auto& shader = m_shaderCache.at(objectPtr->GetObjName());
                shader.Bind();
                shader.SetUniform("model", glm::mat4(1.0f));
                if(glm::all(glm::greaterThanEqual(objectPtr->GetColor(), vec3{0}))){
                    shader.SetUniform("color", objectPtr->GetColor());
                }else{
                    shader.SetUniform("color", vec3(227,75,33) / 255.f);
                }
                if(objectPtr->GetDrawMode() != DrawMode::STATIC){
                    objectPtr->SetupVerticesBuffer();
                    objectPtr->SetPipelineData();
                }
                objectPtr->BindVAO();
                objectPtr->Draw();
                objectPtr->UnBindVAO();
            }
            begin++;
        }
    }




    void setColorUniform(const std::string &shaderName, vec3 color){

    }


    void RenderSystem::setupTextureSamplers() {
        glGenSamplers(1, &m_samplerTexture);
        glSamplerParameteri(m_samplerTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glSamplerParameteri(m_samplerTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glSamplerParameteri(m_samplerTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glSamplerParameteri(m_samplerTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    void RenderSystem::SetProjectionMatrix(const Camera &camera) {
        m_projMatrix = camera.GetProjMatrix(m_width, m_height);
        glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), value_ptr(m_projMatrix));
    }
}