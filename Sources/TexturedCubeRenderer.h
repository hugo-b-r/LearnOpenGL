#ifndef TEXTUREDCUBERENDERER_H
#define TEXTUREDCUBERENDERER_H


#include "CubeTexture.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <tuple>

enum class CubeOrientation {Up, Down, Left, Right, Front, Rear};

class TexturedCubeRenderer {
public:
  TexturedCubeRenderer(CubeAtlas ca = CubeAtlas::Grass);
  void render(Shader &shader, Camera& cam);
  void add(glm::vec3 pos = glm::vec3(0.f), CubeOrientation orientation = CubeOrientation::Front);

private:
  Model m_quadModel;

  bool m_rotating;
  float width = 1;
  float height = 1;
  float length = 1;
  CubeTexture tex;
  // cube atlas, view, model
  std::vector<std::tuple<glm::mat4, glm::mat4>> cubes;

};

#endif
