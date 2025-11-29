#ifndef NONTEXTUREDCUBERENDERER_H
#define NONTEXTUREDCUBERENDERER_H

#include "../deps/glad/include/glad/glad.h"

#include <vector>

class CubeRenderer {
  public:
    CubeRenderer();
    void render (const Camera& camera);
  protected:
    std::vector<GLfloat> vertexCoords;

};

#endif
