#include "NonTexturedCubeRenderer.h"
#include "Camera.h"
#include "Shader.h"
#include <GLFW/glfw3.h>

const int CUBE_NB_VERTICES = 36;

NonTexturedCubeRenderer::NonTexturedCubeRenderer() {
  m_transforModel = glm::mat4(1.f);
  m_quadModel.loadData({{
                            -0.5, -0.5, -0.5, // A
                            0.5, -0.5, -0.5, // B
                            0.5, -0.5, 0.5, // C
                            0.5, 0.5, 0.5, //D
                            0.5, 0.5, -0.5, // E
                            -0.5, 0.5, -0.5, //F
                            -0.5, 0.5, 0.5, //G
                            -0.5, -0.5, 0.5 // H
                        },
                        {},
                        {
                            0, 1, 4, // ABE
                            1, 2, 3, // BCD
                            2, 7, 6, // CHG
                            7, 0, 5, // HAF
                            4, 5, 6, // EFG
                            1, 2, 7, // BCH
                            0, 5, 4, // ABE
                            1, 4, 3, // BCD
                            2, 3, 6, // CHG
                            7, 6, 5, // HAF
                            4, 3, 6, // EFG
                            1, 0, 7, // BCH
                        }});
}

void NonTexturedCubeRenderer::render(Shader &shader, Camera &cam) {
  shader.use();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // send matrices for rendering
  m_quadModel.bindVAO();
  // make sure to initialize matrix to identity matrix first
  // m_transforModel = glm::rotate(m_transforModel, glm::radians(-0.5f),
  // glm::vec3(0.0f, 0.0f, 1.0f));

  //
  glm::mat4 model = glm::mat4(1.0f);

  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f),
                      glm::vec3(0.4f, 0.2f, 1.0f));
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
  projection = cam.getProjMatrix();

  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);

  // render container
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, &);
  glDrawElements(GL_TRIANGLES, CUBE_NB_VERTICES, GL_UNSIGNED_INT, 0);
}
