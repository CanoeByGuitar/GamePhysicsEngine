//
// Created by 王晨辉 on 2023/6/2.
//

#include "RenderSystem.h"
#include "GLShaderProgramFactory.h"
#include "Input.h"

namespace control {
extern vec4 clear_color;
}

namespace renderer {
RenderSystem::RenderSystem() {
  //            m_shaders = {
  //                    "test",
  //                    {
  //                            {std::filesystem::current_path()/"../../" /
  //                            "resource/shader/test/test.frag",
  //                             "fragment"},
  //                            {std::filesystem::current_path()/"../../" /
  //                            "resource/shader/test/test.vert",
  //                             "vertex"}
  //                    }
  //            };
}

void RenderSystem::Init() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    spdlog::info("Failed to initialize GLAD");
    std::abort();
  }

  const auto width  = 1280;
  const auto height = 720;

  m_width  = width;
  m_height = height;

  setupTextureSamplers();
  setDefaultState();

  // Create uniform buffer object for projection and view matrices
  // so same data shared to multiple shader programs.
  for (auto const& item : m_shaderCache) {
    auto   programID     = item.second.GetProgramID();
    GLuint uboBlockIndex = glGetUniformBlockIndex(programID, "Matrices");
    glUniformBlockBinding(programID, uboBlockIndex, 0);
  }

  glGenBuffers(1, &m_uboMatrices);
  glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
  glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboMatrices, 0, 2 * sizeof(glm::mat4));
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glViewport(0, 0, width, height);

  glClearColor(control::clear_color.x * control::clear_color.w,
               control::clear_color.y * control::clear_color.w,
               control::clear_color.z * control::clear_color.w,
               control::clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
}

void RenderSystem::LoadStaticObjects(RenderSystem::RenderListIterator renderListBegin,
                                     RenderSystem::RenderListIterator renderListEnd) {
  auto begin = renderListBegin;
  while (begin != renderListEnd) {
    auto objectPtr = *begin;
    objectPtr->SetupVerticesBuffer();
    objectPtr->SetPipelineData();
    if (objectPtr->m_material && !objectPtr->m_material->m_texturePath.empty()) {
      objectPtr->m_material->LoadTexture();
      // TODO: use texture unit to support multi texture of one mesh
    }
    begin++;
  }
}

void RenderSystem::Update(const Camera& camera) {
  // Window size changed.
  if (Input::GetInstance().ShouldResize()) {
    m_width  = Input::GetInstance().GetWidth();
    m_height = Input::GetInstance().GetHeight();
    glViewport(0, 0, m_width, m_height);
  }

  // Update view matrix inside UBO
  UpdateView(camera);
}

void RenderSystem::Shutdown() const {}

void RenderSystem::UpdateView(const Camera& camera) {
  const auto view = camera.GetViewMatrix();
  glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
}

void RenderSystem::Render(const Camera& camera, RenderSystem::RenderListIterator renderListBegin,
                          RenderSystem::RenderListIterator renderListEnd) {
  setDefaultState();
  //        PHY_DEBUG("clear color in renderer: {} in addr: ", control::clear_color);
  //        std::cout << &control::clear_color << std::endl;
  glClearColor(control::clear_color.x * control::clear_color.w,
               control::clear_color.y * control::clear_color.w,
               control::clear_color.z * control::clear_color.w,
               control::clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  glClear(GL_DEPTH_BUFFER_BIT);
  glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);

  //    renderObjectsNoTextures(renderListBegin, renderListEnd);
  renderObjectsWithTextures(renderListBegin, renderListEnd);
}

void RenderSystem::CompileShaders(RenderListIterator renderListBegin,
                                  RenderListIterator renderListEnd) {
  m_shaderCache.clear();

  auto begin = renderListBegin;
  while (begin != renderListEnd) {
    const auto&              objectPtr = *begin;
    std::string              name      = objectPtr->m_name;
    std::vector<ShaderStage> stages    = {
      {std::filesystem::current_path() / "../../" / "resource/shader" / name / (name + ".frag"),
          "fragment"},
      {std::filesystem::current_path() / "../../" / "resource/shader" / name / (name + ".vert"),
          "vertex"}};
    auto shaderProgram = GLShaderProgramFactory::createShaderProgram(name, stages);

    PHY_ASSERT(shaderProgram, "shader compile error!");
    m_shaderCache.try_emplace(name, std::move(shaderProgram.value()));

    begin++;
  }
}

void RenderSystem::setDefaultState() {
  //        glFrontFace(GL_CCW);
  //        glCullFace(GL_BACK);
  //        glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  //    glDepthMask(GL_TRUE);
  //    glDepthFunc(GL_LEQUAL);
  //    glEnable(GL_BLEND);
  //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RenderSystem::renderObjectsWithTextures(RenderSystem::RenderListIterator renderListBegin,
                                             RenderSystem::RenderListIterator renderListEnd) const {
  auto begin = renderListBegin;
  while (begin != renderListEnd) {
    auto objectPtr = *begin;
    if (objectPtr->m_isVisible) {
      auto& shader = m_shaderCache.at(objectPtr->m_name);
      shader.Bind();
      shader.SetUniform("model", glm::mat4(1.0f));
      shader.SetUniform("color", objectPtr->m_material->m_color);
      if (objectPtr->m_drawMode != DrawMode::STATIC) {
        objectPtr->SetupVerticesBuffer();
        objectPtr->SetPipelineData();
      }
      objectPtr->BindVAO();
      if (objectPtr->m_material && objectPtr->m_material->m_texture != 0) {
        glBindTexture(GL_TEXTURE_2D, objectPtr->m_material->m_texture);
      }
      objectPtr->Draw();
      objectPtr->UnBindVAO();
    }
    begin++;
  }
}

void RenderSystem::renderObjectsNoTextures(RenderListIterator renderListBegin,
                                           RenderListIterator renderListEnd) const {
  auto begin = renderListBegin;
  while (begin != renderListEnd) {
    auto objectPtr = *begin;
    if (objectPtr->m_isVisible) {
      auto& shader = m_shaderCache.at(objectPtr->m_name);
      shader.Bind();
      shader.SetUniform("model", glm::mat4(1.0f));
      shader.SetUniform("color", objectPtr->m_material->m_color);
      if (objectPtr->m_drawMode != DrawMode::STATIC) {
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

void setColorUniform(const std::string& shaderName, vec3 color) {}

void RenderSystem::setupTextureSamplers() {
  glGenSamplers(1, &m_samplerTexture);
  glSamplerParameteri(m_samplerTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glSamplerParameteri(m_samplerTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glSamplerParameteri(m_samplerTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glSamplerParameteri(m_samplerTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void RenderSystem::SetProjectionMatrix(const Camera& camera) {
  m_projMatrix = camera.GetProjMatrix(m_width, m_height);
  glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), value_ptr(m_projMatrix));
}
}   // namespace renderer