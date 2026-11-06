#ifndef NONTEXTUREDCUBERENDERER_H
#define NONTEXTUREDCUBERENDERER_H

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

/// Renderere for opengl cube with textures for each face
class NonTexturedCubeRenderer {
public:
  /// Default constructor
  NonTexturedCubeRenderer(glm::vec3 pos = glm::vec3(0.f), bool rotating = false);
  /// render to opengl
  void render(Shader &shader, Camera& cam);

private:
  /// the quad's model on gpu
  Model m_quadModel;

  /// The quad's pos
  glm::vec3 m_pos;
  /// Wether the quad is rotating or not
  bool m_rotating;
};

#endif
