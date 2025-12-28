#ifndef QUADRENDERER_H
#define QUADRENDERER_H

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

class QuadRenderer {
public:
  QuadRenderer();
  void render(Shader &shader, Camera& cam);

private:
  Model m_quadModel;

  glm::mat4 m_transforModel;
};

#endif // QUADRENDERER_H
