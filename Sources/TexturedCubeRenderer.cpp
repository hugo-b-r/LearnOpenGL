#include "TexturedCubeRenderer.h"
#include "Camera.h"
#include "CubeTexture.h"
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include "../deps/glm/glm/gtx/string_cast.hpp"

const int CUBE_NB_VERTICES = 36;

TexturedCubeRenderer::TexturedCubeRenderer(CubeAtlas ca)
    : tex("./Assets/texture_atlas.jpg") {

  float &w = width;
  float &h = height;
  float &l = length;

  float select = (float) ca;
  float vertical_factor = tex.FactorHeight();
  spdlog::info("Vertical factor : {}", vertical_factor);
  std::vector<GLfloat> vertices = {
      // --- FACE HAUT (Tuile 1: U=[0.0, 0.166667]) ---
      -0.5f * w, 0.5f * h, -0.5f * l, 0.0f, (1.0f + select) * vertical_factor ,
      0.5f * w, 0.5f * h, -0.5f * l, 0.166667f, (1.0f  + select) * vertical_factor ,
      0.5f * w, 0.5f * h, 0.5f * l, 0.166667f, (0.0f + select) * vertical_factor ,
      0.5f * w, 0.5f * h, 0.5f * l, 0.166667f, (0.0f + select) * vertical_factor ,
      -0.5f * w, 0.5f * h, 0.5f * l, 0.0f, (0.0f + select) * vertical_factor ,
      -0.5f * w, 0.5f * h, -0.5f * l, 0.0f, (1.0f + select) * vertical_factor ,

      // --- FACE AVANT (Tuile 2: U=[0.166667, 0.333333]) ---
      -0.5f * w, -0.5f * h, 0.5f * l, 0.166667f, (1.0f + select) * vertical_factor ,
      0.5f * w, -0.5f * h, 0.5f * l, 0.333333f, (1.0f + select) * vertical_factor ,
      0.5f * w, 0.5f * h, 0.5f * l, 0.333333f, (0.0f + select) * vertical_factor ,
      0.5f * w, 0.5f * h, 0.5f * l, 0.333333f, (0.0f + select) * vertical_factor ,
      -0.5f * w, 0.5f * h, 0.5f * l, 0.166667f, (0.0f + select) * vertical_factor ,
      -0.5f * w, -0.5f * h, 0.5f * l, 0.166667f, (1.0f + select) * vertical_factor ,

      // --- FACE GAUCHE (Tuile 3: U=[0.333333, 0.5]) ---
      -0.5f * w, -0.5f * h, 0.5f * l, 0.333333f, (1.0f + select) * vertical_factor ,
      -0.5f * w, -0.5f * h, -0.5f * l, 0.5f, (1.0f + select) * vertical_factor ,
      -0.5f * w, 0.5f * h, -0.5f * l, 0.5f, (0.0f + select) * vertical_factor ,
      -0.5f * w, 0.5f * h, -0.5f * l, 0.5f, (0.0f + select) * vertical_factor ,
      -0.5f * w, 0.5f * h, 0.5f * l, 0.333333f, (0.0f + select) * vertical_factor ,
      -0.5f * w, -0.5f * h, 0.5f * l, 0.333333f, (1.0f + select) * vertical_factor ,

      // --- FACE ARRIÈRE (Tuile 4: U=[0.5, 0.666667]) ---
      -0.5f * w, -0.5f * h, -0.5f * l, 0.666667f, (1.0f + select) * vertical_factor , // Ordre de coins inversé pour l'orientation
      0.5f * w, -0.5f * h, -0.5f * l, 0.5f, (1.0f + select) * vertical_factor ,
      0.5f * w, 0.5f * h, -0.5f * l, 0.5f, (0.0f + select) * vertical_factor ,
      0.5f * w, 0.5f * h, -0.5f * l, 0.5f, (0.0f + select) * vertical_factor ,
      -0.5f * w, 0.5f * h, -0.5f * l, 0.666667f, (0.0f + select) * vertical_factor ,
      -0.5f * w, -0.5f * h, -0.5f * l, 0.666667f, (1.0f + select) * vertical_factor ,

      // --- FACE DROITE (Tuile 5: U=[0.666667, 0.833333]) ---
      0.5f * w, -0.5f * h, -0.5f * l, 0.833333f, (1.0f + select) * vertical_factor , // Ordre de coins ajusté pour l'orientation
      0.5f * w, -0.5f * h, 0.5f * l, 0.666667f, (1.0f + select) * vertical_factor ,
      0.5f * w, 0.5f * h, 0.5f * l, 0.666667f, (0.0f + select) * vertical_factor ,
      0.5f * w, 0.5f * h, 0.5f * l, 0.666667f, (0.0f + select) * vertical_factor ,
      0.5f * w, 0.5f * h, -0.5f * l, 0.833333f, (0.0f + select) * vertical_factor ,
      0.5f * w, -0.5f * h, -0.5f * l, 0.833333f, (1.0f + select) * vertical_factor ,

      // --- FACE DESSOUS (Tuile 6: U=[0.833333, 1.0]) ---
      -0.5f * w, -0.5f * h, 0.5f * l, 0.833333f, (0.0f + select) * vertical_factor , // Ordre de coins ajusté
      0.5f * w, -0.5f * h, 0.5f * l, 1.0f, (0.0f + select) * vertical_factor ,
      0.5f * w, -0.5f * h, -0.5f * l, 1.0f, (1.0f + select) * vertical_factor ,
      0.5f * w, -0.5f * h, -0.5f * l, 1.0f, (1.0f + select) * vertical_factor ,
      -0.5f * w, -0.5f * h, -0.5f * l, 0.833333f, (1.0f + select) * vertical_factor ,
      -0.5f * w, -0.5f * h, 0.5f * l, 0.833333f, (0.0f + select)  * vertical_factor
  };

  // std::vector<GLfloat> vertices2 = {
  //     // --- FACE HAUT (Tuile 1: U=[0.0, 0.166667]) ---
  //     -0.5f * w, 0.5f * h, -0.5f * l, 0.0f, 1.0f ,
  //     0.5f * w, 0.5f * h, -0.5f * l, 0.166667f, 1.0f,
  //     0.5f * w, 0.5f * h, 0.5f * l, 0.166667f, 0.0f ,
  //     0.5f * w, 0.5f * h, 0.5f * l, 0.166667f, 0.0f ,
  //     -0.5f * w, 0.5f * h, 0.5f * l, 0.0f, 0.0f ,
  //     -0.5f * w, 0.5f * h, -0.5f * l, 0.0f, 1.0f ,

  //     // --- FACE AVANT (Tuile 2: U=[0.166667, 0.333333]) ---
  //     -0.5f * w, -0.5f * h, 0.5f * l, 0.166667f, 1.0f ,
  //     0.5f * w, -0.5f * h, 0.5f * l, 0.333333f, 1.0f,
  //     0.5f * w, 0.5f * h, 0.5f * l, 0.333333f, 0.0f ,
  //     0.5f * w, 0.5f * h, 0.5f * l, 0.333333f, 0.0f ,
  //     -0.5f * w, 0.5f * h, 0.5f * l, 0.166667f, 0.0f ,
  //     -0.5f * w, -0.5f * h, 0.5f * l, 0.166667f, 1.0f ,

  //     // --- FACE GAUCHE (Tuile 3: U=[0.333333, 0.5]) ---
  //     -0.5f * w, -0.5f * h, 0.5f * l, 0.333333f, 1.0f ,
  //     -0.5f * w, -0.5f * h, -0.5f * l, 0.5f, 1.0f ,
  //     -0.5f * w, 0.5f * h, -0.5f * l, 0.5f, 0.0f ,
  //     -0.5f * w, 0.5f * h, -0.5f * l, 0.5f, 0.0f ,
  //     -0.5f * w, 0.5f * h, 0.5f * l, 0.333333f, 0.0f ,
  //     -0.5f * w, -0.5f * h, 0.5f * l, 0.333333f, 1.0f,

  //     // --- FACE ARRIÈRE (Tuile 4: U=[0.5, 0.666667]) ---
  //     -0.5f * w, -0.5f * h, -0.5f * l, 0.666667f, 1.0f , // Ordre de coins inversé pour l'orientation
  //     0.5f * w, -0.5f * h, -0.5f * l, 0.5f, 1.0f,
  //     0.5f * w, 0.5f * h, -0.5f * l, 0.5f, 0.0f ,
  //     0.5f * w, 0.5f * h, -0.5f * l, 0.5f, 0.0f ,
  //     -0.5f * w, 0.5f * h, -0.5f * l, 0.666667f, 0.0f ,
  //     -0.5f * w, -0.5f * h, -0.5f * l, 0.666667f, 1.0f ,

  //     // --- FACE DROITE (Tuile 5: U=[0.666667, 0.833333]) ---
  //     0.5f * w, -0.5f * h, -0.5f * l, 0.833333f, 1.0f, // Ordre de coins ajusté pour l'orientation
  //     0.5f * w, -0.5f * h, 0.5f * l, 0.666667f, 1.0f ,
  //     0.5f * w, 0.5f * h, 0.5f * l, 0.666667f, 0.0f ,
  //     0.5f * w, 0.5f * h, 0.5f * l, 0.666667f, 0.0f ,
  //     0.5f * w, 0.5f * h, -0.5f * l, 0.833333f, 0.0f ,
  //     0.5f * w, -0.5f * h, -0.5f * l, 0.833333f, 1.0f,

  //     // --- FACE DESSOUS (Tuile 6: U=[0.833333, 1.0]) ---
  //     -0.5f * w, -0.5f * h, 0.5f * l, 0.833333f, 0.0f , // Ordre de coins ajusté
  //     0.5f * w, -0.5f * h, 0.5f * l, 1.0f, 0.0f ,
  //     0.5f * w, -0.5f * h, -0.5f * l, 1.0f, 1.0f ,
  //     0.5f * w, -0.5f * h, -0.5f * l, 1.0f, 1.0f,
  //     -0.5f * w, -0.5f * h, -0.5f * l, 0.833333f,1.0f,
  //     -0.5f * w, -0.5f * h, 0.5f * l, 0.833333f, (0.0f + select)  * vertical_factor
  // };


  m_quadModel.loadData({vertices, {}, {}}, {3, 2});
}

void TexturedCubeRenderer::add(glm::vec3 pos, CubeOrientation orientation) {
    glm::mat4 m_view(1.0f);
    glm::mat4 model(1.0f);
    m_view = glm::translate(m_view, pos + glm::vec3(0.5f, 0.5f, 0.5f));
    switch (orientation) {
        case CubeOrientation::Up:
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0., 0., 1.));
        break;
        case CubeOrientation::Down:
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0., 0., 1.));
        break;
        case CubeOrientation::Left:
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0., 1., 0.));
        break;
        case CubeOrientation::Right:
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0., 0., 1.));
        break;
        case CubeOrientation::Front:
        // deja dans la bonne orientation
        break;
        case CubeOrientation::Rear:
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0., 0., 1.));
        break;
    }

  cubes.push_back(std::tuple(m_view, model));
}

void TexturedCubeRenderer::render(Shader &shader, Camera &cam) {
  shader.use();
  m_quadModel.bindVAO();
  tex.bind();

  glm::mat4 projection = glm::mat4(1.0f);
  projection = cam.getProjectionViewMatrix();

  for (auto &cube : cubes) {
    shader.setMat4("model", std::get<1>(cube));
    shader.setMat4("view", std::get<0>(cube));
    shader.setMat4("projection", projection);
    // render container
    glDrawArrays(GL_TRIANGLES, 0, CUBE_NB_VERTICES);
  }
}
