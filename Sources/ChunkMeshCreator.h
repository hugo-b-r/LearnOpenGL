#ifndef CHUNKMESHCREATOR_H
#define CHUNKMESHCREATOR_H

#include "../deps/glm/glm/glm.hpp"


#include "Chunk.h"
#include "CubeTexture.h"
#include "Mesh.h"


class ChunkMeshCreator {
public:
  ChunkMeshCreator();
  ChunkMeshCreator(Chunk& c, CubeTexture& tex);
  int buildMesh(Mesh& m, Chunk& c);

protected:
    bool shouldMakeFace(Chunk& c, glm::vec3 side_block);
    Mesh m_mesh;

    std::vector<GLfloat> upFace(int select, float x, float y, float z) const;
    std::vector<GLfloat> downFace(int select, float x, float y, float z) const;
    std::vector<GLfloat> leftFace(int select, float x, float y, float z) const;
    std::vector<GLfloat> rightFace(int select, float x, float y, float z) const;
    std::vector<GLfloat> frontFace(int select, float x, float y, float z) const;
    std::vector<GLfloat> rearFace(int select, float x, float y, float z) const;

    const float cube_width = 1 , cube_length = 1, cube_height = 1;
    float tex_vertical_factor;

};


#endif
