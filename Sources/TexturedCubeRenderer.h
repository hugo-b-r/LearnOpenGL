#ifndef TEXTUREDCUBERENDERER_H
#define TEXTUREDCUBERENDERER_H

#include "CubeTexture.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

class TexturedCubeRenderer {
public:
  TexturedCubeRenderer(glm::vec3 pos = glm::vec3(0.f), bool rotating = false);
  void render(Shader &shader, Camera& cam);

private:
  Model m_quadModel;

  glm::mat4 m_view;
  glm::vec3 m_pos;
  bool m_rotating;
  float width = 1;
  float height = 1;
  float length = 1;
  CubeTexture tex;

};

#endif
