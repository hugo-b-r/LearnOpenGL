#ifndef CHUNK_H
#define CHUNK_H

#include "Camera.h"
#include "CubeTexture.h"
#include "Mathematics/AABB.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

/// Dimension of chunk : width
const int CHUNK_WIDTH = 16;
/// Dimension of chunk : height
const int CHUNK_HEIGHT = 256;
/// Dimension of chunk : length
const int CHUNK_LENGTH = 16;

/// Status of loaded data
enum class ChunkDataLoadStatus {
        /// Not loaded
        None,
        /// Working / loaded
        Running,
        /// Finished created but not loaded
        FinishedNotLoaded,
        /// Finished and loaded,
        FinishedLoaded
};

class Chunk;
class ChunkMeshCreator;
class ChunkRenderer;
class World;
class RemoveSafeChunk;

/// Holds neighbors with fancy names
struct NeighboringChunks {
        /// in x positive direction
        std::shared_ptr<Chunk> x_p;
        /// in x negative direction
        std::shared_ptr<Chunk> x_m;
        /// in y positive direction
        std::shared_ptr<Chunk> y_p;
        /// in y negative direction
        std::shared_ptr<Chunk> y_m;
};

/// A piece of the world
///
/// Because the world is infinite, we subdivide it into smaller pieces, called
/// chunks
class Chunk {
      public:
        /// The basic constructor
        Chunk();
        /// Creates a chunk at pos pos in chunk coordinates among other chunks
        Chunk(glm::ivec2 pos);
        /// Creates a chunk at pos (x, y) in chunk coordinates among other
        /// chunks
        Chunk(int x, int y);

        /// virutal destructor empty
        virtual ~Chunk();
        /// Returns block at position \a x,  \a y, \a z. These are in-chunk
        /// coordinates.
        CubeAtlas getBlockAt(int x, int y, int z);
        /// Returns block at position pos. It is a rebranding of #getBlockAt
        /// with a ivec3 vector.
        CubeAtlas getBlockAt(glm::ivec3 pos);
        /// Set the blokc at chunk coordinates \a x, \a y, \a z.
        void setBlockAt(int x, int y, int z, CubeAtlas ca);
        /// Set the blokc at chunk coordinates \a block_pos.
        void setBlockAt(glm::ivec3 block_pos, CubeAtlas ca);

        /// Returns chunk pos in chunks grid.
        glm::ivec2 getChunkPos();
        /// Returns the center block of the chunk
        glm::ivec3 getChunkPosCenter();
        /// Returns the center in a float vector
        glm::vec3 getChunkPosCenter3();

        /// Create the #AABB for this chunk.
        void createBoundingBox();

        /// Returns the #AABB for this chunk. Must have been created before with
        /// #createBoundingBox.
        AABB &getBoundingBox();

        /// Returns if \a block_pos is contained inside what the chunk stores
        /// when \a block_pos refers to a block relative to the origin of the
        /// world (i.e. absolute)
        bool containAbsPos(glm::ivec3 block_pos);
        /// Returns if \a block_pos is contained inside what the chunk stores
        /// when \a block_pos refers to a block relative to the chunk
        bool containRelPos(glm::ivec3 block_pos);

      protected:
        /// Content of the chunk
        std::vector<CubeAtlas> blocks;
        /// Pos of the chunk among other chunks
        glm::ivec2 chunkPos;
        /// model matrix for the chunk
        glm::mat4 m_modelMat;
        /// View matrix for the chunk
        glm::mat4 m_viewMat;
        /// The # AABB for the chunk
        AABB m_boundingBox;
};

#endif
