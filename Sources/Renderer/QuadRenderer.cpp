#include "QuadRenderer.h"
#include "Camera.h"
#include "Shader.h"
#include <GLFW/glfw3.h>


QuadRenderer::QuadRenderer() {
  m_transforModel = glm::mat4(1.f);
  // m_quadModel.loadData({{0.5f, 0.5f, 0.0f,   // top right
  //                        0.5f, -0.5f, 0.0f,  // bottom right
  //                        -0.5f, -0.5f, 0.0f, // bottom left
  //                        -0.5f, 0.5f, 0.0f},
  //                       {},
  //                       {
  //                           0,
  //                           1,
  //                           3,
  //                           1,
  //                           2,
  //                           3,
  //                       }});
}

void QuadRenderer::render(Shader &shader, Camera &cam) {
  shader.use();
  // send matrices for rendering
  m_quadModel.bindVAO();
  // make sure to initialize matrix to identity matrix first
  // m_transforModel = glm::rotate(m_transforModel, glm::radians(-0.5f),
  // glm::vec3(0.0f, 0.0f, 1.0f));

  //
  glm::mat4 model = glm::mat4(1.0f);

  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.f, 0.0f, 1.0f));
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
  projection = cam.getProjMatrix();

  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);

  // render container
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, &);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
