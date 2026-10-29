#ifndef MESH_H
#define MESH_H

#include "../deps/glad/include/glad/glad.h"
#include <vector>

struct Mesh
{
    std::vector<GLfloat> vertexPositions;
    std::vector<GLfloat> textureCoords;
    std::vector<GLuint>  indices;


    unsigned int getNbVertices() const {
        return vertexPositions.size();
    }
};

#endif // MESH_H_INCLUDED
