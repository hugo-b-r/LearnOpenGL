#include "TexturedCubeRenderer.h"
#include "Camera.h"
#include "Shader.h"
#include <GLFW/glfw3.h>
#include <vector>

const int CUBE_NB_VERTICES = 36;

TexturedCubeRenderer::TexturedCubeRenderer(glm::vec3 pos, bool rotating) : m_pos(pos), m_rotating(rotating), tex("./Assets/texture_small.jpg") {
    glm::mat4 m_view (1.0f);
    m_view = glm::translate(m_view, pos);
    float& w = width;
    float& h = height;
    float& l = length;

    std::vector<GLfloat> vertices = {// --- FACE HAUT (Tuile 1: U=[0.0, 0.166667]) ---
        -0.5f * w,  0.5f * h, -0.5f * l,   0.0f,     1.0f,
         0.5f * w,  0.5f * h, -0.5f * l,   0.166667f,  1.0f,
         0.5f * w,  0.5f * h,  0.5f * l,   0.166667f,  0.0f,
         0.5f * w,  0.5f * h,  0.5f * l,   0.166667f,  0.0f,
        -0.5f * w,  0.5f * h,  0.5f * l,   0.0f,     0.0f,
        -0.5f * w,  0.5f * h, -0.5f * l,   0.0f,     1.0f,

        // --- FACE AVANT (Tuile 2: U=[0.166667, 0.333333]) ---
        -0.5f * w, -0.5f * h,  0.5f * l,   0.166667f,  1.0f,
         0.5f * w, -0.5f * h,  0.5f * l,   0.333333f,  1.0f,
         0.5f * w,  0.5f * h,  0.5f * l,   0.333333f,  0.0f,
         0.5f * w,  0.5f * h,  0.5f * l,   0.333333f,  0.0f,
        -0.5f * w,  0.5f * h,  0.5f * l,   0.166667f,  0.0f,
        -0.5f * w, -0.5f * h,  0.5f * l,   0.166667f,  1.0f,

        // --- FACE GAUCHE (Tuile 3: U=[0.333333, 0.5]) ---
        -0.5f * w, -0.5f * h,  0.5f * l,   0.333333f,  1.0f,
        -0.5f * w, -0.5f * h, -0.5f * l,   0.5f,       1.0f,
        -0.5f * w,  0.5f * h, -0.5f * l,   0.5f,       0.0f,
        -0.5f * w,  0.5f * h, -0.5f * l,   0.5f,       0.0f,
        -0.5f * w,  0.5f * h,  0.5f * l,   0.333333f,  0.0f,
        -0.5f * w, -0.5f * h,  0.5f * l,   0.333333f,  1.0f,

        // --- FACE ARRIÈRE (Tuile 4: U=[0.5, 0.666667]) ---
        -0.5f * w, -0.5f * h, -0.5f * l,   0.666667f,  1.0f, // Ordre de coins inversé pour l'orientation
         0.5f * w, -0.5f * h, -0.5f * l,   0.5f,       1.0f,
         0.5f * w,  0.5f * h, -0.5f * l,   0.5f,       0.0f,
         0.5f * w,  0.5f * h, -0.5f * l,   0.5f,       0.0f,
        -0.5f * w,  0.5f * h, -0.5f * l,   0.666667f,  0.0f,
        -0.5f * w, -0.5f * h, -0.5f * l,   0.666667f,  1.0f,

        // --- FACE DROITE (Tuile 5: U=[0.666667, 0.833333]) ---
         0.5f * w, -0.5f * h, -0.5f * l,   0.833333f,  1.0f, // Ordre de coins ajusté pour l'orientation
         0.5f * w, -0.5f * h,  0.5f * l,   0.666667f,  1.0f,
         0.5f * w,  0.5f * h,  0.5f * l,   0.666667f,  0.0f,
         0.5f * w,  0.5f * h,  0.5f * l,   0.666667f,  0.0f,
         0.5f * w,  0.5f * h, -0.5f * l,   0.833333f,  0.0f,
         0.5f * w, -0.5f * h, -0.5f * l,   0.833333f,  1.0f,

        // --- FACE DESSOUS (Tuile 6: U=[0.833333, 1.0]) ---
        -0.5f * w, -0.5f * h,  0.5f * l,   0.833333f,  0.0f, // Ordre de coins ajusté
         0.5f * w, -0.5f * h,  0.5f * l,   1.0f,       0.0f,
         0.5f * w, -0.5f * h, -0.5f * l,   1.0f,       1.0f,
         0.5f * w, -0.5f * h, -0.5f * l,   1.0f,       1.0f,
        -0.5f * w, -0.5f * h, -0.5f * l,   0.833333f,  1.0f,
        -0.5f * w, -0.5f * h,  0.5f * l,   0.833333f,  0.0f
    };

    m_quadModel.loadData({vertices,{}, {}}, {3, 2});
}

void TexturedCubeRenderer::render(Shader &shader, Camera &cam) {
  shader.use();
  // send matrices for rendering
  m_quadModel.bindVAO();
  // make sure to initialize matrix to identity matrix first

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  if (m_rotating) {
      model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),
                     glm::vec3(0.4f, 0.2f, 1.0f));
  }
  view = glm::translate(view, m_pos);
  projection = cam.getProjectionViewMatrix();
  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);

  // render container
  glDrawArrays(GL_TRIANGLES, 0, CUBE_NB_VERTICES);
}
