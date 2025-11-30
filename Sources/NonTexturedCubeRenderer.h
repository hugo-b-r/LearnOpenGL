#ifndef NONTEXTUREDCUBERENDERER_H
#define NONTEXTUREDCUBERENDERER_H

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

class NonTexturedCubeRenderer {
public:
  NonTexturedCubeRenderer(glm::vec3 pos = glm::vec3(0.f), bool rotating = false);
  void render(Shader &shader, Camera& cam);

private:
  Model m_quadModel;

  glm::mat4 m_view;
  glm::vec3 m_pos;
  bool m_rotating;
};

#endif
