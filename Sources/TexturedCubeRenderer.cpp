#include "TexturedCubeRenderer.h"
#include "Camera.h"
#include "CubeTexture.h"
#include "Shader.h"
#include "utils.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include "../deps/glm/glm/glm.hpp"
#include "../deps/glm/glm/gtx/string_cast.hpp"

const int CUBE_NB_VERTICES = 36;

TexturedCubeRenderer::TexturedCubeRenderer()
    : tex("./Assets/texture_small.jpg") {
  float &w = width;
  float &h = height;
  float &l = length;

  std::vector<GLfloat> vertices = {
      // --- FACE HAUT (Tuile 1: U=[0.0, 0.166667]) ---
      -0.5f * w, 0.5f * h, -0.5f * l, 0.0f, 1.0f, 0.5f * w, 0.5f * h, -0.5f * l,
      0.166667f, 1.0f, 0.5f * w, 0.5f * h, 0.5f * l, 0.166667f, 0.0f, 0.5f * w,
      0.5f * h, 0.5f * l, 0.166667f, 0.0f, -0.5f * w, 0.5f * h, 0.5f * l, 0.0f,
      0.0f, -0.5f * w, 0.5f * h, -0.5f * l, 0.0f, 1.0f,

      // --- FACE AVANT (Tuile 2: U=[0.166667, 0.333333]) ---
      -0.5f * w, -0.5f * h, 0.5f * l, 0.166667f, 1.0f, 0.5f * w, -0.5f * h,
      0.5f * l, 0.333333f, 1.0f, 0.5f * w, 0.5f * h, 0.5f * l, 0.333333f, 0.0f,
      0.5f * w, 0.5f * h, 0.5f * l, 0.333333f, 0.0f, -0.5f * w, 0.5f * h,
      0.5f * l, 0.166667f, 0.0f, -0.5f * w, -0.5f * h, 0.5f * l, 0.166667f,
      1.0f,

      // --- FACE GAUCHE (Tuile 3: U=[0.333333, 0.5]) ---
      -0.5f * w, -0.5f * h, 0.5f * l, 0.333333f, 1.0f, -0.5f * w, -0.5f * h,
      -0.5f * l, 0.5f, 1.0f, -0.5f * w, 0.5f * h, -0.5f * l, 0.5f, 0.0f,
      -0.5f * w, 0.5f * h, -0.5f * l, 0.5f, 0.0f, -0.5f * w, 0.5f * h, 0.5f * l,
      0.333333f, 0.0f, -0.5f * w, -0.5f * h, 0.5f * l, 0.333333f, 1.0f,

      // --- FACE ARRIÈRE (Tuile 4: U=[0.5, 0.666667]) ---
      -0.5f * w, -0.5f * h, -0.5f * l, 0.666667f,
      1.0f, // Ordre de coins inversé pour l'orientation
      0.5f * w, -0.5f * h, -0.5f * l, 0.5f, 1.0f, 0.5f * w, 0.5f * h, -0.5f * l,
      0.5f, 0.0f, 0.5f * w, 0.5f * h, -0.5f * l, 0.5f, 0.0f, -0.5f * w,
      0.5f * h, -0.5f * l, 0.666667f, 0.0f, -0.5f * w, -0.5f * h, -0.5f * l,
      0.666667f, 1.0f,

      // --- FACE DROITE (Tuile 5: U=[0.666667, 0.833333]) ---
      0.5f * w, -0.5f * h, -0.5f * l, 0.833333f,
      1.0f, // Ordre de coins ajusté pour l'orientation
      0.5f * w, -0.5f * h, 0.5f * l, 0.666667f, 1.0f, 0.5f * w, 0.5f * h,
      0.5f * l, 0.666667f, 0.0f, 0.5f * w, 0.5f * h, 0.5f * l, 0.666667f, 0.0f,
      0.5f * w, 0.5f * h, -0.5f * l, 0.833333f, 0.0f, 0.5f * w, -0.5f * h,
      -0.5f * l, 0.833333f, 1.0f,

      // --- FACE DESSOUS (Tuile 6: U=[0.833333, 1.0]) ---
      -0.5f * w, -0.5f * h, 0.5f * l, 0.833333f, 0.0f, // Ordre de coins ajusté
      0.5f * w, -0.5f * h, 0.5f * l, 1.0f, 0.0f, 0.5f * w, -0.5f * h, -0.5f * l,
      1.0f, 1.0f, 0.5f * w, -0.5f * h, -0.5f * l, 1.0f, 1.0f, -0.5f * w,
      -0.5f * h, -0.5f * l, 0.833333f, 1.0f, -0.5f * w, -0.5f * h, 0.5f * l,
      0.833333f, 0.0f};

  m_quadModel.loadData({vertices, {}, {}}, {3, 2});
}

void TexturedCubeRenderer::add(glm::vec3 pos, CubeAtlas ca) {
  glm::mat4 m_view(1.0f);
  glm::mat4 view = glm::translate(m_view, pos);
  cubes.push_back(std::tuple(ca, view));
}

void TexturedCubeRenderer::render(Shader &shader, Camera &cam) {
  shader.use();
  m_quadModel.bindVAO();

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  projection = cam.getProjectionViewMatrix();

  for (auto &cube : cubes) {

    shader.setMat4("model", model);
    shader.setMat4("view", std::get<1>(cube));
    shader.setMat4("projection", projection);
    // render container
    glDrawArrays(GL_TRIANGLES, 0, CUBE_NB_VERTICES);
  }
}
