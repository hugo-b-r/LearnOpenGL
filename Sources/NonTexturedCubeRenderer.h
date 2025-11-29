#ifndef NONTEXTUREDCUBERENDERER_H
#define NONTEXTUREDCUBERENDERER_H

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

class NonTexturedCubeRenderer {
public:
  NonTexturedCubeRenderer();
  void render(Shader &shader, Camera& cam);

private:
  Model m_quadModel;

  glm::mat4 m_transforModel;
};

#endif
