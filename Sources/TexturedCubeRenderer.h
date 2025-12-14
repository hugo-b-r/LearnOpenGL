#ifndef TEXTUREDCUBERENDERER_H
#define TEXTUREDCUBERENDERER_H

#include "CubeTexture.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <tuple>

class TexturedCubeRenderer {
public:
  TexturedCubeRenderer();
  void render(Shader &shader, Camera& cam);
  void add(glm::vec3 pos = glm::vec3(0.f), CubeAtlas ca = Grass);

private:
  Model m_quadModel;

  glm::mat4 m_view;
  glm::vec3 m_pos;
  bool m_rotating;
  float width = 1;
  float height = 1;
  float length = 1;
  CubeTexture tex;
  std::vector<std::tuple<CubeAtlas, glm::mat4>> cubes;

};

#endif
