#ifndef CHUNKMESHCREATOR_H
#define CHUNKMESHCREATOR_H

#include <glm/glm.hpp>

#include "CubeTexture.h"
#include "Mesh.h"
#include "World.h"
#include "World/Chunk.h"

enum class ChunkMeshBuildStatus {
        NotStarted,
        Running,
        DoneNotLoaded,
        DoneLoaded
};

class Chunk;
class World;

class ChunkMeshCreator {
      public:
        ChunkMeshCreator(Chunk &c, float tex_factor_height, World &m,
                         Mesh *result_location);
        bool buildMesh();

      protected:
        bool shouldMakeFace(glm::ivec3 side_block, const NeighboringChunks& neighbors);

        CubeAtlas getFromNeighbors(glm::ivec3 block_pos, const NeighboringChunks& neighbors);
        /// push to m_mesh the mesh of a block, checking if it needs to build or
        /// not
        void addBlockMesh(int i, int j, int k, NeighboringChunks &neighbors);

        Mesh *m_mesh;
        std::array<GLfloat, 48> upFace(int select, float x, float y,
                                       float z) const;
        std::array<GLfloat, 48> downFace(int select, float x, float y,
                                         float z) const;
        std::array<GLfloat, 48> leftFace(int select, float x, float y,
                                         float z) const;
        std::array<GLfloat, 48> rightFace(int select, float x, float y,
                                          float z) const;
        std::array<GLfloat, 48> frontFace(int select, float x, float y,
                                          float z) const;
        std::array<GLfloat, 48> rearFace(int select, float x, float y,
                                         float z) const;

        const float cube_width = 1, cube_length = 1, cube_height = 1;
        float tex_vertical_factor;
        Chunk &data;
        ChunkMeshBuildStatus status;

        World &world;
};

#endif
