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
    const float cubeWidth = 16;

    //thanks raylib
    std::vector<GLfloat> vertices = {
           -w/2, h/2, -l/2,// A
           -w/2, h/2, -l/2,// A
           w/2, h/2, -l/2,// B
           w/2, h/2, -l/2,// B
           w/2, h/2, l/2,// C
           w/2, h/2, l/2,// C
           -w/2, h/2, l/2,// D
           -w/2, h/2, l/2,// D
           -w/2, -h/2, l/2,// E
           -w/2, -h/2, l/2,// E
           -w/2, -h/2, -l/2,// F
           -w/2, -h/2, -l/2,// F
           w/2, -h/2, -l/2,// G
           w/2, -h/2, -l/2,// G
           w/2, -h/2, l/2,// H
           w/2, -h/2, l/2,// H
       };

    GLfloat factor_width = tex.FactorWidth();
    GLfloat factor_height = tex.FactorHeight();
       std::vector<GLfloat> texcoords = {
            0.f, 0.f, // A
            48.f/96.f, 16.f/16.f, // A
            16.f/96.f, 0.f, // B
            64.f/96.f, 1.f, // B
            16.f/96.f, 1.f,// C
            5.f/6.f *factor_width, factor_height, // C
            0.f, factor_height, // D
            2.f/6.f *factor_width, factor_height, // D
            2.f/6.f *factor_width, 0.f, // E
            5.f/6.f *factor_width, factor_height, // E
            3.f/6.f *factor_width, 0.f, // F
            6.f/6.f *factor_width, factor_height, // F
            4.f/6.f *factor_width, 0.f, // G
            6.f/6.f *factor_width, 0.f, // G
            1.f/6.f *factor_width, 0.f, // H
            5.f/6.f *factor_width, 0.f, // H
       };
    std::vector<GLuint> indices = {
        0, 2, 6, // ABD
        2, 4, 6, // BCD
        14, 4, 8, // BCE
        4, 7, 8, // CDE
        7, 8, 10, // DEF
        7, 1, 10, // ADF
        1, 12, 10, // AFG
        1, 3, 12, // ABG
        3, 12, 15,// BGH
        3, 5, 15, // BCH
        9, 15, 13, // EHG
        9, 11, 13, // EFG
    };

    m_quadModel.loadData({vertices,texcoords, indices});

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void TexturedCubeRenderer::render(Shader &shader, Camera &cam) {
  shader.use();
  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // send matrices for rendering
  m_quadModel.bindVAO();
  // make sure to initialize matrix to identity matrix first
  // m_transforModel = glm::rotate(m_transforModel, glm::radians(-0.5f),
  // glm::vec3(0.0f, 0.0f, 1.0f));

  //

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
  //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, &);
  glDrawElements(GL_TRIANGLES, CUBE_NB_VERTICES, GL_UNSIGNED_INT, 0);
}
