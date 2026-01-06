#ifndef CHUNKMESHCREATOR_H
#define CHUNKMESHCREATOR_H

#include <glm/glm.hpp>


#include "Chunk.h"
#include "CubeTexture.h"
#include "Mesh.h"
#include "World.h"

enum class ChunkMeshBuildStatus { NotStarted, Running, DoneNotLoaded, DoneLoaded};

class Chunk;
class World;

class ChunkMeshCreator {
public:
  ChunkMeshCreator(Chunk& c, CubeTexture& tex, World& m);
  int buildMesh();

  bool isFinished();
  int nbVertices();

  Mesh getMesh();

protected:
    bool shouldMakeFace(glm::ivec3 side_block);

    int buildMeshThreadFn();

    Mesh m_mesh;
    int nb_vertices;

    std::vector<GLfloat> upFace(int select, float x, float y, float z) const;
    std::vector<GLfloat> downFace(int select, float x, float y, float z) const;
    std::vector<GLfloat> leftFace(int select, float x, float y, float z) const;
    std::vector<GLfloat> rightFace(int select, float x, float y, float z) const;
    std::vector<GLfloat> frontFace(int select, float x, float y, float z) const;
    std::vector<GLfloat> rearFace(int select, float x, float y, float z) const;

    const float cube_width = 1 , cube_length = 1, cube_height = 1;
    float tex_vertical_factor;
    Chunk& data;
    ChunkMeshBuildStatus status;

    World& m_world;


};


#endif
