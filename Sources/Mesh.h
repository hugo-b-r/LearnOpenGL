#ifndef MESH_H
#define MESH_H

#include "../deps/glad/include/glad/glad.h"
#include <vector>


/// A discrete representation of a volume.
struct Mesh
{
    /// The vertexes
    std::vector<GLfloat> vertexPositions;
    /// Some texture coords
    std::vector<GLfloat> textureCoords;
    /// A mapping between #vertexPositions and #textureCoords
    std::vector<GLuint>  indices;


    /// Returns the number of vertices 
    unsigned int getNbVertices() const {
        return vertexPositions.size();
    }
};

#endif // MESH_H_INCLUDED
