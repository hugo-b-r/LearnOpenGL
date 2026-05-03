#ifndef WORLD_H
#define WORLD_H

#include <glm/glm.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>
// in order to have the ability to hash the type for unordered map
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include "Chunk.h"
#include "CubeTexture.h"
#include "Renderer/ChunkRenderer.h"

class Camera;
class Chunk;
class ChunkRenderer;
class DebugInfos;

class World {
      public:
        World(Camera &cam);
        ~World();

        /// procedure to load the world correctly
        void load();

        /// Renders the world using a renderer, according to a camera position,
        /// and whether it uses debuginfos or not
        void render(Camera &cam, ChunkRenderer &renderer, DebugInfos &inf);

        /// returns the block present at the coord pos (assuming they are
        /// absolute) if exists
        CubeAtlas getBlockAt(glm::ivec3 pos);
        /// sets the value for the block present at the coord pos (assuming they
        /// are absolute) if exists
        void setBlockAt(glm::ivec3 pos, CubeAtlas cube);

        void playerLeftClicked(Camera &cam);
        void playerRightClicked(Camera &cam);

        /// checks whether a chunk is loaded or not at the ChunkPosition pos
        bool isLoadedAt(glm::ivec2 pos);
        /// create the mesh for the chunk at the pos pos
        void createAndLoadMeshAt(glm::ivec2 pos);
        /// checks whether a chunk is loaded or not at the ChunkPosition pos
        bool isMeshLoadedAt(glm::ivec2 pos);

        /// returns chunk at the position pos
        Chunk *getRelativeToPlayerAt(glm::ivec2 pos);
        /// returns chunk at the position pos
        Chunk *getRelativeToPlayerAt(int x, int y);

        /// returns the chunk present at a pos diff to player's pos
        Chunk *getAbsoluteAt(glm::ivec2 pos);
        /// returns the chunk present at a pos diff to player's pos
        Chunk *getAbsoluteAt(int x, int y);

        /// set the chunk present at a pos diff to player's pos
        void setRelativeToPlayerAt(glm::ivec2 pos, Chunk *c);
        /// set the chunk present at a pos diff to player's pos
        void setRelativeToPlayerAt(int x, int y, Chunk *c);

      protected:
        /// move all the map one Chunk in x + direction
        void moveChunkXP();
        /// move all the map one Chunk in x - direction
        void moveChunkXM();
        /// move all the map one Chunk in y + direction
        void moveChunkYP();
        /// move all the map one Chunk in y - direction
        void moveChunkYM();

        /// tests wether the chunk is in the boundaries of managed chunks at
        /// position pos (absolute position)
        bool existsAt(glm::ivec2 pos);

        /// The width of the managed zone, i.e., if the render distance is x, it
        /// is 2*(x+1)+1 : there is for each side of the player, a render
        /// distance long of blocks, + 1 chunk border that is loaded but not
        /// shown, and the player is itself in a chunk.
        int chunkWidth;

        /// this is the number of chunks that there are total. It is the size of
        /// the vector 'chunks'
        int chunksNb;

        /// A structure to store chunks. They are keptin a grid and adressed
        /// using their x, y coordinates using this formula:
        ///        'x + y * chunkwidth'
        /// Whenever the player moves to another chunk, all of them ar moved to
        /// another place, using 'moveChunkXP', 'moveChunkXM', 'moveChunkYP' and
        /// 'moveChunkYM'. This is much faster than a map, even unordered, and
        /// easier to iter on. Moreover, I think that using pointer reduces the
        /// number of operations needed to move chunks, as we only move at most
        /// something like 8 bytes x nbChunks which is much less than 256 x 16 x
        /// 16 x nbChunks.
        std::vector<Chunk *> chunks;

        // The world is existing according to a camera. This is used to know
        // where to load chunks for example, but also what to render.
        Camera &camera;
        // The pos where the player was before : pretty useful to know if it
        // moved or not, and to compute the relative position of some chunks to
        // the actual player's position.
        glm::ivec2 lastChunkPlayerPos;
};

#endif
