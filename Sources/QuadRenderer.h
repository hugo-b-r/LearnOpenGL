#ifndef QUADRENDERER_H
#define QUADRENDERER_H

#include "Shader.h"
#include "Model.h"

class QuadRenderer {
public:
  QuadRenderer();
  void render(Shader &shader);

private:
  Model m_quadModel;
};

#endif // QUADRENDERER_H
