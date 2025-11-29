#ifndef MESH_H
#define MESH_H

#include "../deps/glad/include/glad/glad.h"
#include <vector>

struct Mesh
{
    std::vector<GLfloat> vertexPositions;
    std::vector<GLfloat> textureCoords;
    std::vector<GLuint>  indices;
};

#endif // MESH_H_INCLUDED
