#ifndef CHUNKMESHCREATOR_H
#define CHUNKMESHCREATOR_H

#include <glm/glm.hpp>

#include "CubeTexture.h"
#include "Mesh.h"
#include "World.h"
#include "World/Chunk.h"

class Chunk;
class World;

/// Position of neighboring faces
struct NeighbooringFaces {
        /// Change your neighbors
        void update(int x, int y, int z) {
                left = {x - 1, y, z};
                right = {x + 1, y, z};
                up = {x, y + 1, z};
                down = {x, y - 1, z};
                front = {x, y, z + 1};
                rear = {x, y, z - 1};
        }
        /// Face on the 'left'
        glm::ivec3 left;
        /// Face on the 'right'
        glm::ivec3 right;
        /// Face above
        glm::ivec3 up;
        /// Face down
        glm::ivec3 down;
        /// Face front facing
        glm::ivec3 front;
        /// Face rear facing
        glm::ivec3 rear;
};

/// Create a mesh out of a chunk.
///
/// Uses many optmization, such as back face culling, not drawing blocks that
/// are not visible...
class ChunkMeshCreator {
      public:
        /// Default constructor
        ChunkMeshCreator(Chunk &c, float tex_factor_height, World &m,
                         Mesh *result_location);

        /// To build the mesh. Does all the complex logic
        bool buildMesh();

      protected:
        /// Whether we should make a face or not in direction \a side_block
        /// Use \a neighbors to get data on close blocks
        bool shouldMakeFace(glm::ivec3 side_block,
                            const NeighboringChunks &neighbors, CubeAtlas block);

        /// Return block value from neighbor when not accessible from inside the
        /// studied chunk
        CubeAtlas getFromNeighbors(glm::ivec3 block_pos,
                                   const NeighboringChunks &neighbors);
        /// push to m_mesh the mesh of a block, checking if it needs to build or
        /// not
        void addBlockMesh(int i, int j, int k, NeighboringChunks &neighbors);

        /// The built #Mesh
        Mesh *m_mesh;

        /// Coordinates of the up face
        std::array<GLfloat, 48> upFace(int select, float x, float y,
                                       float z) const;
        /// Coordinates of the down face
        std::array<GLfloat, 48> downFace(int select, float x, float y,
                                         float z) const;
        /// Coordinates of the left face
        std::array<GLfloat, 48> leftFace(int select, float x, float y,
                                         float z) const;
        /// Coordinates of the right face
        std::array<GLfloat, 48> rightFace(int select, float x, float y,
                                          float z) const;
        /// Coordinates of the front face
        std::array<GLfloat, 48> frontFace(int select, float x, float y,
                                          float z) const;
        /// Coordinates of the rear face
        std::array<GLfloat, 48> rearFace(int select, float x, float y,
                                         float z) const;

        /// Cube dimensions, used for texturing
        const float cube_width = 1;
        /// Cube dimensions, used for texturing
        const float cube_length = 1;
        /// Cube dimensions, used for texturing
        const float cube_height = 1;
        /// A dimension ratio of the texture, used for texturing
        float tex_vertical_factor;
        /// a reference to the meshed chunk
        Chunk &data;

        /// A reference to the data of the current world
        World &world;
};

#endif
